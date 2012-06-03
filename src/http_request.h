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

namespace bolt {
namespace network {
namespace http {

class Request {
    public:
        Request();
        ~Request();

        typedef std::map<std::string, std::string> headers_t;

        size_t parse(const char *data, const size_t &len);
        bool finished() const;
        bool valid() const;

        void update_body(const char *data, const size_t &len);
        void update_header_field(const char *data, const size_t &len);
        void update_header_value(const char *data, const size_t &len);

        void set_uri(const char *data, const size_t &len);
        void set_body(const char *data, const size_t &len);
        void set_header_field(const char *data, const size_t &len);
        void set_header_value(const char *data, const size_t &len);

        inline bool has_body() const { return !body_.empty(); };
        inline bool has_uri() const { return !uri_.empty(); };
        inline bool has_header_field() const { return !header_field_.empty(); };
        inline bool has_header_value() const { return !header_value_.empty(); };

        std::string header_field() const;
        std::string header_value() const;

        const std::string & path() const;

        void add_header(const std::string &header, const std::string &value);

        void on_message_complete();
    private:
        http_parser parser_;
        size_t body_len_;

        std::string uri_;
        std::string body_;
        headers_t headers_;

        std::string header_field_;
        std::string header_value_;

        struct {
            unsigned parse_finished : 1;
            unsigned keep_alive: 1;
            unsigned body_len_unknown : 1;
            unsigned invalid : 1;
        } state_;

};

}; // namespace http
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_HTTP_REQUEST_H__
