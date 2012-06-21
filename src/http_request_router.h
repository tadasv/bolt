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

#ifndef __BOLT_HTTP_REQUEST_ROUTER__

#define __BOLT_HTTP_REQUEST_ROUTER__

#include <vector>
#include <utility>

#include "http_incomming_connection.h"

namespace bolt {
namespace network {
namespace http {

enum RouteTypes {
    RouteExact = 0,
    RouteStartsWith
};

class RequestRouter {
    public:
        RequestRouter();
        ~RequestRouter();

        typedef void (*request_handler_t)(IncommingConnection *);
        typedef struct {
            std::string path;
            RouteTypes type;
            request_handler_t handler;
        } route_entry_t;
        typedef std::vector<route_entry_t> routes_t;

        void add_route(const std::string &path, RouteTypes type, request_handler_t handler);
        request_handler_t route(const std::string &path);

    private:
        routes_t routes_;
};

}; // namespace http
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_HTTP_REQUEST_ROUTER__
