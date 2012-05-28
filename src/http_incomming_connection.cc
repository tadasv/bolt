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
#include "http_incomming_connection.h"
#include "http_server.h"

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
}

IncommingConnection::IncommingConnection(Server *server, struct ev_loop *loop, int socket, struct sockaddr_in address)
    : server_(server), socket_(socket), address_(address), read_buffer_size_(512), event_loop_(loop)
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

    size_t nparsed = request.parse(read_buffer_, bytes_read);

    if (bytes_read == 0) {
        ev_io_stop(event_loop_, &read_watcher_);
        ev_io_stop(event_loop_, &write_watcher_);
        shutdown(socket_, SHUT_RDWR);
    }

    return bytes_read;
}


Server *IncommingConnection::server()
{
    return server_;
}

}; // namespace http
}; // namespace network
}; // namespace bolt
