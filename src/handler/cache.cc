// Copyright Tadas Vilkeliskis.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.#include "handler/cache.h"

#include "http_server.h"
#include "http_incomming_connection.h"
#include "http_response.h"
#include "string_utils.h"

#include "db/document.h"
#include "db/set.h"
#include "db/set_manager.h"

#include "messages.h"

#include <vector>
#include <jansson.h>

namespace bolt {
namespace network {
namespace http {
namespace handler {

// helper function for writing responses.
static void _write_response(Response &res, const std::string &body)
{
    res.headers["Content-Length"] = bolt::util::string::t_to_string(body.length());
    res.write_head(200);
    res.write(body.c_str(), body.length());
    res.end();
}


static void _show_cache_stats(Request &req, Response &res, bolt::db::SetManager *manager)
{
    if (req.method != bolt::network::http::kMethodGet) {
        _write_response(res, bolt::network::messages::kMessageUnsupportedMethod);
        return;
    }

    int result;
    const bolt::db::set_map_t & sets = manager->sets();
    bolt::db::set_map_t::const_iterator iter;

    json_t *list = json_array();
    for (iter = sets.begin(); iter != sets.end(); ++iter) {
        result = json_array_append_new(list,
                                       json_pack("{s:s}",
                                                 "name",
                                                  iter->first.c_str()));
        if (result != 0) {
            json_decref(list);
            _write_response(res, bolt::network::messages::kMessageInternalError);
            return;
        }
    }

    json_t *json = json_pack("{s:{s:o}}",
                             "data",
                             "sets",
                             list);
    if (!json) {
        json_decref(list);
        _write_response(res, bolt::network::messages::kMessageInternalError);
        return;
    }

    std::string body = std::string(json_dumps(json, JSON_COMPACT));
    _write_response(res, body);

    json_decref(json);
}


static void _create_document(Request &req,
                             Response &res,
                             std::string &set_name,
                             bolt::db::SetManager *manager)
{
    bolt::db::Document *doc = new bolt::db::Document();
    bolt::db::DocumentError error = doc->parse(req.body.c_str());

    if (error != bolt::db::kDocumentOK) {
        delete doc;
        _write_response(res, bolt::network::messages::kMessageInvalidRequest);
        return;
    }

    bolt::db::Set *set = manager->find(set_name);
    if (!set) {
        set = new bolt::db::Set(set_name);
        manager->add(set);
    }

    if (!set->add(doc)) {
        delete doc;
        _write_response(res, bolt::network::messages::kMessageInternalError);
        return;
    }

    std::string body = std::string("{\"_id\":\"") + doc->id() + std::string("\"}");
    _write_response(res, body);
}


void cache(IncommingConnection *connection)
{
    Response &res = connection->response;
    Request &req = connection->request;

    const std::string & path = connection->request.url.path();
    const char *tmp = path.c_str() + 7; // +7 to skip /cache/

    bolt::util::string::string_tokens_t tokens = bolt::util::string::string_get_tokens(tmp, '/');

    res.headers["Content-Type"] = "application/json";

    switch (tokens.size()) {
        case 0:
            // Show cache stats
            _show_cache_stats(req, res, connection->server()->manager());
            return;
            break;
        case 1:
            {
                bolt::util::string::string_offset_t & off = tokens[0];
                std::string set_name = std::string(tmp + off.first, off.second);

                if (connection->request.method == bolt::network::http::kMethodPost) {
                    // create a new document in this set.
                    //
                    _create_document(req, res, set_name,
                                     connection->server()->manager());
                    return;
                }

                if (!connection->server()->manager()->find(set_name)) {
                    _write_response(res, bolt::network::messages::kMessageUnknownSet);
                    return;
                }

                if (connection->request.method == bolt::network::http::kMethodGet) {
                    // Show stats about the set
                    json_t *json = json_pack("{s:{s:s}}",
                                             "data",
                                             "set",
                                             set_name.c_str());
                    _write_response(res, std::string(json_dumps(json, JSON_COMPACT)));
                    json_decref(json);
                    return;
                }

                _write_response(res, bolt::network::messages::kMessageUnsupportedMethod);
                return;

            }
            break;
        case 2:
            // show exact document
            {
                bolt::util::string::string_offset_t & off = tokens[0];
                std::string set_name = std::string(tmp + off.first, off.second);
                off = tokens[1];
                std::string doc_id = std::string(tmp + off.first, off.second);
                _write_response(res, std::string("\"showing '" + set_name + "' / '" + doc_id + "'\""));
                return;
            }
            break;
        default:
            _write_response(res, bolt::network::messages::kMessageInvalidRequest);
            break;
    }
}

}; // namespace handler
}; // namespace http
}; // namespace network
}; // namespace bolt