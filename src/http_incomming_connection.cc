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

    if (connection->request.has_finished()) {
        RequestRouter *router = connection->server()->router();
        if (!router) {
            // Kill the connection right way since we don't have any handler
            // set up
            connection->server()->remove_connection(connection);
            delete connection;
            return;
        }

        const std::string &path = connection->request.path();
        RequestRouter::request_handler_t handler = router->route(path);

        // Handle the request
        if (handler) {
            handler(connection);
        }

        if (connection->response.finished()) {
            const std::string data = connection->response.data();
            connection->write(data.c_str(), data.length());
        }

        connection->server()->remove_connection(connection);
        delete connection;
    } else if (connection->request.is_valid() == false) {
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

    ev_io_init(&read_watcher_, bolt::network::http::read_callback, socket_, EV_READ);
    ev_io_init(&write_watcher_, bolt::network::http::write_callback, socket_, EV_WRITE);

    read_watcher_.data = this;
    write_watcher_.data = this;

    read_buffer_ = new char[read_buffer_size_];

    ev_io_start(event_loop_, &read_watcher_);
    ev_io_start(event_loop_, &write_watcher_);
}


IncommingConnection::~IncommingConnection()
{
    bolt_log_debug("Destoying connection %p", this);

    ev_io_stop(event_loop_, &read_watcher_);
    ev_io_stop(event_loop_, &write_watcher_);

    shutdown(socket_, SHUT_RDWR);

    delete [] read_buffer_;
}

int IncommingConnection::read()
{
    ssize_t bytes_read;

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
    }

    return bytes_read;
}


int IncommingConnection::write(const char *data, const size_t &len)
{
    ssize_t bytes_written;

    bytes_written = send(socket_, data, len, 0);

    return bytes_written;
}


Server *IncommingConnection::server()
{
    return server_;
}

}; // namespace http
}; // namespace network
}; // namespace bolt
