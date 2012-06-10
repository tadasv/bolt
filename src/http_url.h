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

#ifndef __BOLT_HTTP_URL__

#define __BOLT_HTTP_URL__

#include <string>

namespace bolt {
namespace network {
namespace http {

class Url {
    public:
        Url();
        Url(const char *url, const size_t &len);
        Url(const std::string &url, const size_t &len);
        Url(const char *url);
        Url(const std::string &url);

        Url(const std::string &schema, const std::string &host,
            const int &port, const std::string &path,
            const std::string &query, const std::string &fragment);

        Url(const char *schema, const char *host, const int &port,
            const char *path, const char *query, const char *fragment);

        inline void set_schema(const std::string &str) { schema_ = str; };
        inline void set_host(const std::string &str) { host_ = str; };
        inline void set_port(const unsigned int &port) { port_ = port; };
        inline void set_path(const std::string &str) { path_ = str; };
        inline void set_query(const std::string &str) { query_ = str; };
        inline void set_fragment(const std::string &str) { fragment_ = str; };

        inline const std::string & schema() const { return schema_; };
        inline const std::string & host() const { return host_; };
        inline const unsigned int port() const { return port_; };
        inline const std::string & path() const { return path_; };
        inline const std::string & query() const { return query_; };
        inline const std::string & fragment() const { return fragment_; };

        bool parse(const char *url, size_t len);

        inline bool parsed() const { return parsed_; };
    private:
        void init_(const std::string &schema="http",
                   const std::string &host="",
                   const int &port=80,
                   const std::string &path="",
                   const std::string &query="",
                   const std::string &fragment="");

        std::string schema_;
        std::string host_;
        int port_;
        std::string path_;
        std::string query_;
        std::string fragment_;
        bool parsed_;
};

}; // namespace http
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_HTTP_URL__
