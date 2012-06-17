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

#include <stdio.h>

#include "logging.h"
#include "http_server.h"
#include "http_incomming_connection.h"
#include "http_request_router.h"

namespace bolt {
namespace network {
namespace http {

// After how many seconds to kill the connection if there is no activity.
static const int kConnectionTimeout = 60;

static void timeout_callback(struct ev_loop *loop, ev_timer *timer, int revents)
{
    IncommingConnection *connection = static_cast<IncommingConnection*>(timer->data);

    if (connection->timeout()) {
        connection->server()->remove_connection(connection);
        delete connection;
    }
}

static void read_callback(struct ev_loop *loop, ev_io *io, int revents)
{
    IncommingConnection *connection = static_cast<IncommingConnection*>(io->data);
    int bytes_read = connection->read();

    if (bytes_read < 0) {
    }

    if (bytes_read == 0) {
        // Close connection
        connection->server()->remove_connection(connection);
        delete connection;
    }

}

static void write_callback(struct ev_loop *loop, ev_io *io, int wevents)
{

    IncommingConnection *connection = static_cast<IncommingConnection*>(io->data);

    if (connection->request.state == kFinished) {
        RequestRouter *router = connection->server()->router();
        if (!router) {
            // Kill the connection right away since we don't have any handler
            // set up
            connection->server()->remove_connection(connection);
            delete connection;
            return;
        }

        const std::string &path = connection->request.url.path();
        RequestRouter::request_handler_t handler = router->route(path);

        // Handle the request
        if (handler) {
            handler(connection);
        }

        // Write data to socket if buffer is not empty
        // TODO in the future we might want to send data in
        // chunks and call handler asynchronously.
        if (connection->response.buffer.empty() == false) {
            connection->write(connection->response.buffer.c_str(),
                              connection->response.buffer.length());
        }

        // Close connection if the handler is finished.
        if (connection->response.state == kResponseFinished) {
            connection->server()->remove_connection(connection);
            delete connection;
        }
    } else if (connection->request.state == kErrorInvalid) {
        // bad request
        std::string res = std::string(
            "HTTP/1.1 400 Bad Request\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "shit went south\r\n\r\n");

        connection->write(res.c_str(), res.length());
        connection->server()->remove_connection(connection);
        delete connection;
    }
}

IncommingConnection::IncommingConnection(Server *server, struct ev_loop *loop, int socket, struct sockaddr_in address)
    : server_(server), socket_(socket), read_buffer_size_(512), address_(address), event_loop_(loop)
{
    bolt_log_debug("New connection %p", this);

    read_buffer_ = new char[read_buffer_size_];

    ev_io_init(&read_watcher_, bolt::network::http::read_callback, socket_, EV_READ);
    read_watcher_.data = this;
    ev_io_start(event_loop_, &read_watcher_);

    ev_io_init(&write_watcher_, bolt::network::http::write_callback, socket_, EV_WRITE);
    write_watcher_.data = this;
    ev_io_start(event_loop_, &write_watcher_);

    last_activity_ = ev_now(event_loop_);
    ev_init(&timeout_timer_, timeout_callback);
    timeout_timer_.data = this;

    timeout_callback(event_loop_, &timeout_timer_, 0);
}


IncommingConnection::~IncommingConnection()
{
    bolt_log_debug("Destoying connection %p", this);

    ev_timer_stop(event_loop_, &timeout_timer_);

    ev_io_stop(event_loop_, &read_watcher_);
    ev_io_stop(event_loop_, &write_watcher_);

    shutdown(socket_, SHUT_RDWR);
    close(socket_);

    delete [] read_buffer_;
}

int IncommingConnection::read()
{
    ssize_t bytes_read;

    last_activity_ = ev_now(event_loop_);
    bytes_read = recv(socket_, read_buffer_, read_buffer_size_, 0);

    if (bytes_read < 0) {
        printf("failed to read data\n");
        return bytes_read;
    }

    request.parse(read_buffer_, bytes_read);

    if (bytes_read == 0) {
        ev_io_stop(event_loop_, &read_watcher_);
        ev_io_stop(event_loop_, &write_watcher_);
        shutdown(socket_, SHUT_RDWR);
        close(socket_);
    }

    return bytes_read;
}


int IncommingConnection::write(const char *data, const size_t &len)
{
    ssize_t bytes_written;

    last_activity_ = ev_now(event_loop_);
    bytes_written = send(socket_, data, len, 0);

    return bytes_written;
}


Server *IncommingConnection::server()
{
    return server_;
}


bool IncommingConnection::timeout()
{
    ev_tstamp after = last_activity_ - ev_now(event_loop_) + kConnectionTimeout;
    if (after < 0.0) {
        return true;
    }

    ev_timer_set(&timeout_timer_, after, 0.0);
    ev_timer_start(event_loop_, &timeout_timer_);

    return false;
}

}; // namespace http
}; // namespace network
}; // namespace bolt
