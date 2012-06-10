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

#include "logging.h"
#include "http_request.h"

namespace bolt {
namespace network {
namespace http {

static int on_message_begin(http_parser *parser);
static int on_uri(http_parser *parser, const char *at, size_t len);
static int on_header_field(http_parser *parser, const char *at, size_t len);
static int on_header_value(http_parser *parser, const char *at, size_t len);
static int on_headers_complete(http_parser *parser);
static int on_body(http_parser *parser, const char *at, size_t len);
static int on_message_complete(http_parser *parser);

static const http_parser_settings kParserSettings = {
    on_message_begin,
    on_uri,
    on_header_field,
    on_header_value,
    on_headers_complete,
    on_body,
    on_message_complete
};


Request::Request()
{
    state = kNotStarted;
    content_length = ULLONG_MAX;
    http_parser_init(&parser_, HTTP_REQUEST);
    parser_.data = this;
}


size_t Request::parse(const char *data, const size_t &len)
{
    size_t nparsed = http_parser_execute(&parser_, &kParserSettings, data, len);

    if (nparsed != len) {
        state = kErrorInvalid;
    }

    return nparsed;
}


static int on_message_begin(http_parser *parser)
{
    Request *request = static_cast<Request*>(parser->data);
    request->state = kProcessing;
    return 0;
}


static int on_uri(http_parser *parser, const char *at, size_t len)
{
    if (len == 0 || !at) {
        return -1;
    }

    Request *request = static_cast<Request*>(parser->data);
    if (!request->url.parse(at, len)) {
        request->state = kErrorInvalid;
        return -1;
    }

    return 0;
}


static int on_header_field(http_parser *parser, const char *at, size_t len)
{
    Request *request = static_cast<Request*>(parser->data);

    if (!request->header_value_.empty()) {
        request->headers[request->header_field_] = request->header_value_;
        request->header_value_.clear();
        request->header_field_ = std::string(at, len);
        return 0;
    }

    request->header_field_ += std::string(at, len);

    return 0;
}


static int on_header_value(http_parser *parser, const char *at, size_t len)
{
    Request *request = static_cast<Request*>(parser->data);

    if (!request->header_value_.empty()) {
        request->header_value_ += std::string(at, len);
    } else {
        request->header_value_ = std::string(at, len);
    }

    return 0;
}


static int on_headers_complete(http_parser *parser)
{
    Request *request = static_cast<Request*>(parser->data);
    request->content_length = parser->content_length;
    return 0;
}


static int on_body(http_parser *parser, const char *at, size_t len)
{
    Request *request = static_cast<Request*>(parser->data);


    if (request->body.empty()) {
        if (request->content_length == ULLONG_MAX) {
            // If we did not receive the Content-Length header
            // don't process the body and finish the request
            // right away,
            return -1;
        }
        request->body = std::string(at, len);
    } else {
        request->body += std::string(at, len);
    }

    return 0;
}


static int on_message_complete(http_parser *parser)
{
    Request *request = static_cast<Request*>(parser->data);
    request->state = kFinished;
    return 0;
}

}; // namesapce http
}; // namespace network
}; // namespace bolt
