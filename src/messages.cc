#include "messages.h"

namespace bolt {
namespace network {
namespace messages {

// Static response messages
const std::string kMessageInvalidRequest("{\"error\":\"invalid request.\"}");
const std::string kMessageUnsupportedMethod("{\"error\":\"unsupported method.\"}");
const std::string kMessageInternalError("{\"error\":\"internal error.\"}");
const std::string kMessageUnknownSet("{\"error\":\"unknown set.\"}");

}; // namespace messages
}; // namespace network
}; // namespace bolt
