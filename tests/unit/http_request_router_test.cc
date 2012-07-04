#include <gtest/gtest.h>

#include "http/request_router.h"

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
    router.add_route("/A", bolt::network::http::RouteExact, handler_A);
    handler = router.route("/A");
    ASSERT_TRUE(handler_A == handler);
    router.add_route("/A", bolt::network::http::RouteExact, handler_B);
    handler = router.route("/A");
    ASSERT_TRUE(handler_A == handler);
    router.add_route("/B", bolt::network::http::RouteExact, handler_B);
    handler = router.route("/B");
    ASSERT_TRUE(handler_B == handler);
    router.add_route("/C/", bolt::network::http::RouteStartsWith, handler_B);
    handler = router.route("/C/");
    ASSERT_TRUE(handler_B == handler);
    handler = router.route("/C/123");
    ASSERT_TRUE(handler_B == handler);
}
