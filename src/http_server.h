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

#ifndef __BOLT_HTTP_SERVER_H__

#define __BOLT_HTTP_SERVER_H__

#include <netinet/in.h>
#include <ev.h>

#include <map>

#include "types.h"
#include "http_incomming_connection.h"
#include "http_request_router.h"

namespace bolt {
namespace network {
namespace http {

class Server {
    public:
        Server(unsigned port, struct ev_loop *loop);
        ~Server();

        typedef std::map<const IncommingConnection*, const IncommingConnection*> connections_t;

        bolt::core::BoltResult init();
        bolt::core::BoltResult start();
        bolt::core::BoltResult stop();

        IncommingConnection *accept_connection();

        void add_connection(const IncommingConnection *connection);
        void remove_connection(const IncommingConnection *connection);

        void set_router(RequestRouter *router);
        RequestRouter *router() const;

        int listening_socket() const;
        struct ev_loop *event_loop() const;

    protected:
        unsigned port_number_;
        int socket_;
        bool init_complete_;
        struct sockaddr_in addr_;
        struct ev_loop *event_loop_;
        struct ev_io accept_watcher_;
        connections_t connections_;
        RequestRouter *router_;
};

}; // namespace http
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_HTTP_SERVER_H__
