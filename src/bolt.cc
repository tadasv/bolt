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
#include <ev.h>

#include "logging.h"
#include "http_server.h"
#include "http_request_router.h"

int main(int argc, char **argv)
{
    struct ev_loop *loop = EV_DEFAULT;
    struct ev_loop *loop_socket = NULL;
    ev_embed embed;

    if (ev_supported_backends() &
        ~ev_recommended_backends() &
        EVBACKEND_KQUEUE) {
        loop_socket = ev_loop_new(EVBACKEND_KQUEUE);
        if (loop_socket) {
            ev_embed_init(&embed, 0, loop_socket);
            ev_embed_start(loop, &embed);
        }
    }

    if (!loop_socket) {
        loop_socket = loop;
    }

    bolt::network::http::RequestRouter router;

    bolt::network::http::Server server(3333, loop_socket);

    if (server.init() != bolt::core::kResultOK) {
        printf("Failed to initialise server.\n");
        return -1;
    }

    server.set_router(&router);

    if (server.start() != bolt::core::kResultOK) {
        printf("Failed to start server.\n");
        return -1;
    }

    ev_run(loop, 0);

    return 0;
}
