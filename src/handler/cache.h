#ifndef __BOLT_HTTP_HANDLER_CACHE_H__

#define __BOLT_HTTP_HANDLER_CACHE_H__

namespace bolt {
namespace network {
namespace http {

    class IncommingConnection;

namespace handler {
    
    void cache(IncommingConnection *connection);

}; // namespace handler
}; // namespace http
}; // namespace network
}; // namespace bolt

#endif // end of include guard: __BOLT_HTTP_HANDLER_CACHE_H__
