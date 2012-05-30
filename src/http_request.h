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
