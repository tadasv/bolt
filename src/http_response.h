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

#ifndef __BOLT_HTTP_RESPONSE_H__

#define __BOLT_HTTP_RESPONSE_H__

#include <string>

#include "http_message.h"

namespace bolt {
namespace network {
namespace http {

enum ResponseStates {
    kResponseNotStarted = 0,
    kResponseHeadersBuffered,
    kResponseDataBuffered,
    kResponseFinished
};

class Response {
    public:
        Response();

        void write_head(const unsigned int &status_code);
        void write(const char *str, const size_t &len);
        void end(const char *str, const size_t &len);
        void end();

        // Status code that will be used for implicit header write.
        unsigned int status_code;
        // Response state. This field should not be modified directly.
        ResponseStates state;
        // Response buffer
        std::string buffer;
        // Response headers
        headers_t headers;
};

}; // namespace http
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_HTTP_RESPONSE_H__
