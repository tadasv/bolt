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

#include "db/document.h"

namespace bolt {
namespace db {

const int kInfiniteLifetimeTTL = -1;

void generate_uuid(uuid_t id)
{
    uuid_generate(id);
}


std::string uuid_to_str(uuid_t id)
{
    uuid_string_t str;
    uuid_unparse(id, str);
    return std::string(str);
}


std::string generate_uuid_str()
{
    uuid_t id;
    generate_uuid(id);
    return uuid_to_str(id);
}


bool str_to_uuid(const char *str, uuid_t id)
{
    return (uuid_parse(str, id) == 0);
}


Document::Document()
{
    root_ = NULL;
    ttl_ = -1;
}


Document::~Document()
{
    if (root_) {
        json_decref(root_);
        root_ = NULL;
    }
}


const std::string & Document::id() const
{
    return id_;
}


bool Document::to_string(std::string &json_string) const
{
    if (!root_) {
        return false;
    }

    char *str = json_dumps(root_, JSON_COMPACT);
    if (!str) {
        return false;
    }

    json_string = std::string(str);
    return true;
}


DocumentError Document::parse(const std::string &str)
{
    return parse(str.c_str());
}


DocumentError Document::parse(const char *str)
{
    json_t *json = NULL;
    json_error_t error;

    json = json_loads(str, 0, &error);
    if (!json) {
        return kDocumentErrorParseFailed;
    }

    // Validate that root is object
    if (!json_is_object(json)) {
        json_decref(json);
        return kDocumentErrorObjectExpected;
    }

    // Get the document ID if available or generate
    // a new one on the fly.
    json_t *doc_id = json_object_get(json, "_id");
    if (!doc_id) {
        id_ = generate_uuid_str();
        doc_id = json_string(id_.c_str());
        if (json_object_set_new(json, "_id", doc_id) == -1) {
            json_decref(json);
            return kDocumentError;
        }
    } else {
        if (!json_is_string(doc_id)) {
            // _id can only be a string, exit early if
            // anything else.
            json_decref(json);
            return kDocumentErrorInvalidID;
        }
        id_ = std::string(json_string_value(doc_id));
    }

    // Extract TTL. Don't try to be too smart about it. We only support
    // integer TTL. If anything else is passed in or TTL is < -1 exit
    // early. If TTL is not passed in, implicitly assume infinite lifetime.
    json_t *ttl = json_object_get(json, "_ttl");
    if (!ttl) {
        ttl_ = kInfiniteLifetimeTTL;
        if (json_object_set_new(json, "_ttl", json_integer(ttl_)) == -1) {
            json_decref(json);
            return kDocumentError;
        }
    } else {
        if (!json_is_integer(ttl)) {
            json_decref(json);
            return kDocumentErrorInvalidTTL;
        }

        ttl_ = json_integer_value(ttl);
        if (ttl_ < -1) {
            json_decref(json);
            return kDocumentErrorInvalidTTL;
        }
    }

    // Release previous root if calling parse
    // multiple times.
    if (root_) {
        json_decref(root_);
    }
    root_ = json;
    return kDocumentOK;
}


}; // namespace db
}; // namespace bolt
