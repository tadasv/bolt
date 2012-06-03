#include <gtest/gtest.h>

#include "http_request.h"

static const std::string REQUEST_A = std::string(
"GET /path?a=1&b=2&_ HTTP/1.1\r\n"
"Connection: close\r\n"
"Content-Length: 12\r\n"
"Header: value\r\n"
"\r\n"
"this is body\r\n"
"\r\n"
);

TEST(HttpRequestTestCase, ParserTest)
{
    bolt::network::http::Request *req;
    size_t nparsed;

    req = new bolt::network::http::Request();
    nparsed = req->parse(REQUEST_A.c_str(), REQUEST_A.length());
    ASSERT_EQ(nparsed, REQUEST_A.length());
    ASSERT_TRUE(req->is_valid());
    ASSERT_TRUE(req->has_finished());
    ASSERT_TRUE(req->has_body());
    ASSERT_TRUE(req->has_uri());
    ASSERT_STREQ("this is body", req->body().c_str());
    delete req;

    // Parse in two chunks
    req = new bolt::network::http::Request();
    nparsed = req->parse(REQUEST_A.c_str(), REQUEST_A.length() / 2);
    ASSERT_EQ(nparsed, REQUEST_A.length() / 2);
    ASSERT_TRUE(req->is_valid());
    ASSERT_FALSE(req->has_finished());
    ASSERT_FALSE(req->has_body());
    ASSERT_TRUE(req->has_uri());

    nparsed = req->parse(REQUEST_A.c_str() + REQUEST_A.length() / 2,
                         REQUEST_A.length() - REQUEST_A.length() / 2);
    ASSERT_EQ(nparsed, REQUEST_A.length() - REQUEST_A.length() / 2);
    ASSERT_TRUE(req->is_valid());
    ASSERT_TRUE(req->has_finished());
    ASSERT_TRUE(req->has_body());
    ASSERT_TRUE(req->has_uri());
    ASSERT_STREQ("this is body", req->body().c_str());
    delete req;
}
