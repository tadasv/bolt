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

#include <http_parser.h>
#include "http/url.h"

namespace bolt {
namespace network {
namespace http {

static inline bool http_parser_valid_field(const http_parser_url *p, http_parser_url_fields field)
{
    return (p->field_set & (1 << field));
}

static inline void http_parser_field_data(const http_parser_url *p,
                                          http_parser_url_fields field,
                                          uint16_t *off, uint16_t *len)
{
    *off = p->field_data[field].off;
    *len = p->field_data[field].len;
}


Url::Url()
{
    init_("http", "", 80);
}


Url::Url(const char *url, const size_t &len)
{
    parse(url, len);
}


Url::Url(const char *url)
{
    parse(url, strlen(url));
}


Url::Url(const std::string &url)
{
    parse(url.c_str(), url.length());
}

Url::Url(const std::string &schema, const std::string &host,
         const int &port, const std::string &path, const std::string &query,
         const std::string &fragment)
{
    init_(schema, host, port, path, query, fragment);
}


Url::Url(const char *schema, const char *host, const int &port,
         const char *path, const char *query, const char *fragment)
{
    init_(std::string(schema),
          std::string(host),
          port,
          std::string(path),
          std::string(query),
          std::string(fragment));
}


void Url::init_(const std::string &schema,
                const std::string &host,
                const int &port,
                const std::string &path,
                const std::string &query,
                const std::string &fragment)
{
    schema_ = schema;
    host_ = host;
    port_ = port;
    path_ = path;
    query_ = query;
    fragment_ = fragment;
    parsed_ = false;
}


bool Url::parse(const char *url, size_t length)
{
    struct http_parser_url parsed_url;
    uint16_t offset;
    uint16_t len;

    port_ = 0;
    parsed_ = false;

    if (http_parser_parse_url(url,
                              length,
                              0, // is_connect,
                              &parsed_url) != 0) {
        return false;
    }

    if (http_parser_valid_field(&parsed_url, UF_SCHEMA)) {
        http_parser_field_data(&parsed_url, UF_SCHEMA, &offset, &len);
        schema_ = std::string(url + offset, len);
    }

    if (http_parser_valid_field(&parsed_url, UF_HOST)) {
        http_parser_field_data(&parsed_url, UF_HOST, &offset, &len);
        host_ = std::string(url + offset, len);
    }

    if (http_parser_valid_field(&parsed_url, UF_HOST)) {
        port_ = parsed_url.port;
    }

    if (http_parser_valid_field(&parsed_url, UF_PATH)) {
        http_parser_field_data(&parsed_url, UF_PATH, &offset, &len);
        path_ = std::string(url + offset, len);
    }

    if (http_parser_valid_field(&parsed_url, UF_QUERY)) {
        http_parser_field_data(&parsed_url, UF_QUERY, &offset, &len);
        query_ = std::string(url + offset, len);
    }

    if (http_parser_valid_field(&parsed_url, UF_FRAGMENT)) {
        http_parser_field_data(&parsed_url, UF_FRAGMENT, &offset, &len);
        fragment_ = std::string(url + offset, len);
    }
 
    parsed_ = true;
    return true;
}


}; // namespace http
}; // namespace network
}; // namespace bolt
