#include <gtest/gtest.h>

#include "http_request_router.h"

class bolt::network::http::IncommingConnection;

void handler_A(bolt::network::http::IncommingConnection *c)
{
    return;
}

void handler_B(bolt::network::http::IncommingConnection *c)
{
    return;
}


TEST(HttpRequestRouterTestCase, RoutesTest)
{
    bolt::network::http::RequestRouter router;
    bolt::network::http::RequestRouter::request_handler_t handler;

    ASSERT_TRUE(router.route("/some/route"));
    router.set_route("/A", handler_A);
    handler = router.route("/A");
    ASSERT_TRUE(handler_A == handler);
    router.set_route("/A", handler_B);
    handler = router.route("/A");
    ASSERT_TRUE(handler_B == handler);
}
