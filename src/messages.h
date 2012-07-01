#ifndef __BOLT_NETWORK_MESSAGES_H__

#define __BOLT_NETWORK_MESSAGES_H__

#include <string>

namespace bolt {
namespace network {
namespace messages {

extern const std::string kMessageInvalidRequest;
extern const std::string kMessageUnsupportedMethod;
extern const std::string kMessageInternalError;
extern const std::string kMessageUnknownSet;

}; // namespace messages
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_NETWORK_MESSAGES_H__
