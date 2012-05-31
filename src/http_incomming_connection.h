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

#ifndef __BOLT_HTTP_INCOMMING_CONNECTION_H__

#define __BOLT_HTTP_INCOMMING_CONNECTION_H__

#include <netinet/in.h>
#include <ev.h>

#include "http_request.h"
#include "http_response.h"

namespace bolt {
namespace network {
namespace http {

class Server;

class IncommingConnection {
    public:
        IncommingConnection(Server *server, struct ev_loop *loop, int socket, struct sockaddr_in address);
        ~IncommingConnection();

        int read();
        int write(const char *data, const size_t &len);

        Server *server();

        Request request;
        Response response;
    protected:
        Server *server_;
        int socket_;
        ssize_t read_buffer_size_;
        char *read_buffer_;
        struct sockaddr_in address_;
        struct ev_io read_watcher_;
        struct ev_io write_watcher_;
        struct ev_loop *event_loop_;
    private:
};

}; // namespace http
}; // namespace network
}; // namespace bolt


#endif // end of include guard: __BOLT_INCOMMING_CONNECTION_H__
