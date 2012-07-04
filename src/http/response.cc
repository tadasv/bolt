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

#include "http/response.h"
#include "string_utils.h"

namespace bolt {
namespace network {
namespace http {

Response::Response()
{
    state = kResponseNotStarted;
    status_code = 200;
}


void Response::write_head(const unsigned int &status_code)
{
    // Headers must be sent first, so don't do
    // anything if we are trying to send headers
    // in another state.
    if (state != kResponseNotStarted) {
        return;
    }

    buffer += "HTTP/1.1 " + bolt::util::string::t_to_string(status_code) + "\r\n";

    headers_t::iterator iter;
    for (iter = headers.begin(); iter != headers.end(); iter++) {
        buffer += iter->first + ": " + iter->second + "\r\n";
    }
    buffer += "\r\n";

    state = kResponseHeadersBuffered;
}


void Response::write(const char *str, const size_t &len)
{
    if (state == kResponseNotStarted) {
        write_head(status_code);
    }

    buffer += std::string(str, len);
}


void Response::end()
{
    end(NULL, 0);
}


void Response::end(const char *str, const size_t &len)
{
    if (str && len) {
        write(str, len);
    }

    if (state == kResponseNotStarted) {
        write_head(status_code);
    }

    state = kResponseFinished;
}


}; // namespace http
}; // namespace network
}; // namespace bolt
