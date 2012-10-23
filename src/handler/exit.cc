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

#include "handler/exit.h"

#include "http/incomming_connection.h"
#include "http/request.h"
#include "http/response.h"

#include <ev.h>

extern struct ev_loop *global_loop;

namespace bolt {
namespace network {
namespace http {
namespace handler {

void exit(IncommingConnection *connection)
{
    ev_unloop(global_loop, EVUNLOOP_ALL);

    Response &res = connection->response;
    res.headers["Content-Type"] = "application/json";
    res.headers["Content-Length"] = 2;
    res.write_head(200);
    res.write("{}", 2);
    res.end();
}
    
}; // namespace handler
}; // namespace http
}; // namespace network
}; // namespace bolt
