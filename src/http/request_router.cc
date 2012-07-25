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
// USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "http/request_router.h"
#include "core/string_utils.h"

namespace bolt {
namespace network {
namespace http {


static void default_handler(IncommingConnection *connection)
{
    Response &res = connection->response;

    std::string body("{\"error\":\"invalid request\"}");

    res.headers["Content-Type"] = "application/json";
    res.headers["Content-Length"] = bolt::util::string::t_to_string(body.length());

    res.write_head(400);
    res.write(body.c_str(), body.length());
    res.end();
}


RequestRouter::RequestRouter()
{
}


RequestRouter::~RequestRouter()
{
}


void RequestRouter::add_route(const std::string &path, RouteTypes type, request_handler_t handler)
{
    route_entry_t entry;

    entry.path = path;
    entry.type = type;
    entry.handler = handler;
    routes_.push_back(entry);
}


RequestRouter::request_handler_t RequestRouter::route(const std::string &path)
{
    routes_t::iterator iter;

    for (iter = routes_.begin(); iter != routes_.end(); ++iter) {
        switch (iter->type) {
            case RouteExact:
                if (path == iter->path) {
                    return iter->handler;
                }
                break;
            case RouteStartsWith:
                if (path.compare(0, iter->path.length(), iter->path) == 0) {
                    return iter->handler;
                }
                break;
            default:
                return default_handler;
        }
    }
    return default_handler;
}

}; // namespace http
}; // namespace network
}; // namespace bolt
