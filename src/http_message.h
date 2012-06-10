#ifndef __BOLT_HTTP_MESSAGE_H__

#define __BOLT_HTTP_MESSAGE_H__

#include <string>
#include <map>

namespace bolt {
namespace network {
namespace http {

typedef std::map<std::string, std::string> headers_t;

class Message {
    public:
        std::string start_line;
        headers_t headers;
        std::string body;
};

}; // namespace http
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_HTTP_MESSAGE_H__
