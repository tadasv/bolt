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

#include "http_request_router.h"
#include "string_utils.h"

namespace bolt {
namespace network {
namespace http {


static void default_handler(IncommingConnection *connection)
{
    Response &res = connection->response;

    std::string body("{\"error\":\"invalid request\"}");

    res.headers["Content-Type"] = "application/json";
    res.headers["Content-Length"] = t_to_string(body.length());

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


void RequestRouter::set_route(const std::string &path, request_handler_t handler)
{
    routes_[path] = handler;
}


RequestRouter::request_handler_t RequestRouter::route(const std::string &path)
{
    routes_t::iterator iter = routes_.find(path);
    if (iter != routes_.end()) {
        return iter->second;
    }
    return default_handler;
}

}; // namespace http
}; // namespace network
}; // namespace bolt
