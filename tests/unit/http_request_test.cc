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


static const std::string REQUEST_B = std::string(
"GET /path?a=1&b=2&_ HTTP/1.1\r\n"
"Connection: close\r\n"
"Header: value\r\n"
"\r\n"
);

// Invalid HTTP method
static const std::string REQUEST_C = std::string(
"SEARCH /path?a=1&b=2&_ HTTP/1.1\r\n"
"Connection: close\r\n"
"Header: value\r\n"
"\r\n"
);

TEST(HttpRequestTestCase, ParserTest)
{
    bolt::network::http::Request *req;
    size_t nparsed;

    req = new bolt::network::http::Request();
    nparsed = req->parse(REQUEST_A.c_str(), REQUEST_A.length());
    ASSERT_EQ(nparsed, REQUEST_A.length());
    ASSERT_EQ(req->state, bolt::network::http::kFinished);
    ASSERT_FALSE(req->body.empty());
    ASSERT_STREQ("this is body", req->body.c_str());
    ASSERT_EQ(12, req->content_length);
    ASSERT_EQ(bolt::network::http::kMethodGet, req->method);
    delete req;

    // Parse in two chunks
    req = new bolt::network::http::Request();
    nparsed = req->parse(REQUEST_A.c_str(), REQUEST_A.length() / 2);
    ASSERT_EQ(nparsed, REQUEST_A.length() / 2);
    ASSERT_NE(req->state, bolt::network::http::kFinished);
    ASSERT_TRUE(req->body.empty());

    nparsed = req->parse(REQUEST_A.c_str() + REQUEST_A.length() / 2,
                         REQUEST_A.length() - REQUEST_A.length() / 2);
    ASSERT_EQ(nparsed, REQUEST_A.length() - REQUEST_A.length() / 2);
    ASSERT_EQ(req->state, bolt::network::http::kFinished);
    ASSERT_FALSE(req->body.empty());
    ASSERT_STREQ("this is body", req->body.c_str());
    ASSERT_EQ(12, req->content_length);
    ASSERT_EQ(bolt::network::http::kMethodGet, req->method);
    delete req;

    // If request contain no Content-Length we won't parse the body.
    req = new bolt::network::http::Request();
    nparsed = req->parse(REQUEST_B.c_str(), REQUEST_B.length());
    ASSERT_EQ(nparsed, REQUEST_B.length());
    ASSERT_EQ(req->state, bolt::network::http::kFinished);
    ASSERT_TRUE(req->body.empty());
    ASSERT_EQ(ULLONG_MAX, req->content_length);
    ASSERT_EQ(bolt::network::http::kMethodGet, req->method);
    delete req;

    req = new bolt::network::http::Request();
    nparsed = req->parse(REQUEST_C.c_str(), REQUEST_C.length());
    ASSERT_EQ(nparsed, REQUEST_C.length());
    ASSERT_EQ(req->state, bolt::network::http::kFinished);
    ASSERT_TRUE(req->body.empty());
    ASSERT_EQ(ULLONG_MAX, req->content_length);
    ASSERT_EQ(bolt::network::http::kMethodUnknown, req->method);
    delete req;
}
