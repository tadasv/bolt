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

#ifndef __BOLT_HTTP_REQUEST_H__

#define __BOLT_HTTP_REQUEST_H__

#include <string>
#include <map>

#include <http_parser.h>
#include "http/message.h"
#include "http/url.h"

namespace bolt {
namespace network {
namespace http {

enum RequestStates {
    kNotStarted = 0,
    kProcessing,
    kFinished,
    kErrorInvalid
};


enum RequestMethods {
    kMethodGet = 0,
    kMethodPost,
    kMethodPut,
    kMethodDelete,
    kMethodUnknown
};

class Request : public Message {
    public:
        Request();

        size_t parse(const char *data, const size_t &len);

        RequestStates state;
        size_t content_length;
        Url url;
        RequestMethods method;

        // Temporary header data that we use while parsing
        // a request.
        std::string header_field_;
        std::string header_value_;

    private:
        http_parser parser_;
};

}; // namespace http
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_HTTP_REQUEST_H__
