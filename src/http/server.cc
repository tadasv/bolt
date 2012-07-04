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

#include <cassert>
#include <fcntl.h>

#include "logging.h"
#include "http/server.h"
#include "http/incomming_connection.h"

namespace bolt {
namespace network {
namespace http {

static void accept_callback(struct ev_loop *loop, ev_io *watcher, int revents)
{
    if (EV_ERROR & revents) {
        bolt_log_error("Error in read event for listening socket.");
        return;
    }

    bolt::network::http::Server *server = static_cast<bolt::network::http::Server*>(watcher->data);
    server->accept_connection();
}

Server::Server(unsigned port, struct ev_loop *loop)
    : port_number_(port), socket_(-1), init_complete_(false), event_loop_(loop),
      router_(0)
{
}

Server::~Server()
{
    if (socket_ >= 0) {
        shutdown(socket_, SHUT_RDWR);
        close(socket_);
    }
}

bolt::core::BoltResult Server::init()
{
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port_number_);
    addr_.sin_addr.s_addr = INADDR_ANY;

    accept_watcher_.data = this;
    init_complete_ = true;

    return bolt::core::kResultOK;
}


bolt::core::BoltResult Server::start()
{
    int res;

    assert(init_complete_);

    if (!init_complete_) {
        return bolt::core::kResultError;
    }

    socket_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ < 0) {
        return bolt::core::kResultNetworkError;
    }

    // Set SO_REUSEADDR s othat we could reuse the IP address.
    int optval = 1;
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    res = bind(socket_, (struct sockaddr*)&addr_, sizeof(addr_));
    if (res != 0) {
        shutdown(socket_, SHUT_RDWR);
        close(socket_);
        socket_ = -1;
        return bolt::core::kResultNetworkError;
    }

    if (listen(socket_, 128) < 0) {
        return bolt::core::kResultNetworkError;
    }

    ev_io_init(&accept_watcher_,
               accept_callback,
               socket_,
               EV_READ);
    ev_io_start(event_loop_, &accept_watcher_);

    bolt_log_info("Server listening on port %d", port_number_);

    return bolt::core::kResultOK;
}


bolt::core::BoltResult Server::stop()
{
    ev_io_stop(event_loop_, &accept_watcher_);
    shutdown(socket_, SHUT_RDWR);
    close(socket_);

    // TODO close all connections

    init();

    return bolt::core::kResultOK;
}


void Server::set_router(RequestRouter *router)
{
    router_ = router;
}


RequestRouter *Server::router() const
{
    return router_;
}


void Server::set_manager(bolt::db::SetManager *manager)
{
    manager_ = manager;
}


bolt::db::SetManager *Server::manager() const
{
    return manager_;
}


IncommingConnection* Server::accept_connection()
{
    IncommingConnection *connection;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;

    client_socket = accept(socket_, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        bolt_log_error("Failed to accept incomming connection.");
        return 0;
    }

    int flags = fcntl(client_socket, F_GETFL, 0);
    if (fcntl(client_socket, F_SETFL, (flags < 0 ? 0 : flags) | O_NONBLOCK) == -1) {
        bolt_log_debug("Failed to set nonblocking client socket.");
        shutdown(client_socket, SHUT_RDWR);
        close(client_socket);
        return 0;
    }

    // TODO instead of creating a instance of new connection here
    // put all necessary data for creating IncommingConnection in a thread
    // safe queue where worker threads can create and handle connections.
    // TODO realese connection object when it's done.
    connection = new IncommingConnection(this, event_loop_, client_socket, client_addr);
    add_connection(connection);
    return connection;
}


void Server::add_connection(const IncommingConnection *connection)
{
    bolt_log_debug("Adding connection %p to server %p.", connection, this);
    connections_[connection] = connection;
}


void Server::remove_connection(const IncommingConnection *connection)
{
    connections_t::iterator iter = connections_.find(connection);
    if (iter != connections_.end()) {
        bolt_log_debug("Removing connection %p from server %p.", connection, this);
        connections_.erase(iter);
    }
}


int Server::listening_socket() const
{
    return socket_;
}


struct ev_loop *Server::event_loop() const
{
    return event_loop_;
}

}; // namespace http
}; // namespace network
}; // namespace bolt
