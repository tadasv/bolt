#include <gtest/gtest.h>

#include "http_url.h"

namespace http = bolt::network::http;

TEST(HttpUrlTestCase, ConstructorTest)
{
    http::Url url;

    ASSERT_STREQ("http", url.schema().c_str());
    ASSERT_STREQ("", url.host().c_str());
    ASSERT_EQ(80, url.port());
    ASSERT_STREQ("", url.path().c_str());
    ASSERT_STREQ("", url.query().c_str());
    ASSERT_STREQ("", url.fragment().c_str());

    url = http::Url(std::string("https"),
                    std::string("localhost"),
                    443,
                    std::string("/"),
                    std::string("a=b&b=c"),
                    std::string("this is fragment"));
    ASSERT_FALSE(url.parsed());
    ASSERT_STREQ("https", url.schema().c_str());
    ASSERT_STREQ("localhost", url.host().c_str());
    ASSERT_EQ(443, url.port());
    ASSERT_STREQ("/", url.path().c_str());
    ASSERT_STREQ("a=b&b=c", url.query().c_str());
    ASSERT_STREQ("this is fragment", url.fragment().c_str());

    url = http::Url("https",
                    "localhost",
                    443,
                    "/",
                    "a=b&b=c",
                    "this is fragment");
    ASSERT_FALSE(url.parsed());
    ASSERT_STREQ("https", url.schema().c_str());
    ASSERT_STREQ("localhost", url.host().c_str());
    ASSERT_EQ(443, url.port());
    ASSERT_STREQ("/", url.path().c_str());
    ASSERT_STREQ("a=b&b=c", url.query().c_str());
    ASSERT_STREQ("this is fragment", url.fragment().c_str());

    url = http::Url("http://localhost/a/path/?a=b#fragment");
    ASSERT_TRUE(url.parsed());
    ASSERT_STREQ("http", url.schema().c_str());
    ASSERT_STREQ("localhost", url.host().c_str());
    ASSERT_EQ(0, url.port());
    ASSERT_STREQ("/a/path/", url.path().c_str());
    ASSERT_STREQ("a=b", url.query().c_str());
    ASSERT_STREQ("fragment", url.fragment().c_str());

    url = http::Url("https://localhost:555/a/path/#fragment");
    ASSERT_TRUE(url.parsed());
    ASSERT_STREQ("https", url.schema().c_str());
    ASSERT_STREQ("localhost", url.host().c_str());
    ASSERT_EQ(555, url.port());
    ASSERT_STREQ("/a/path/", url.path().c_str());
    ASSERT_STREQ("", url.query().c_str());
    ASSERT_STREQ("fragment", url.fragment().c_str());

    url = http::Url("/a/path/?a=b#fragment");
    ASSERT_TRUE(url.parsed());
    ASSERT_STREQ("", url.schema().c_str());
    ASSERT_STREQ("", url.host().c_str());
    ASSERT_EQ(0, url.port());
    ASSERT_STREQ("/a/path/", url.path().c_str());
    ASSERT_STREQ("a=b", url.query().c_str());
    ASSERT_STREQ("fragment", url.fragment().c_str());

    url = http::Url("a/path/?a=b#fragment");
    ASSERT_FALSE(url.parsed());

    url = http::Url(std::string("https://localhost:555/a/path/#fragment"));
    ASSERT_TRUE(url.parsed());
    ASSERT_STREQ("https", url.schema().c_str());
    ASSERT_STREQ("localhost", url.host().c_str());
    ASSERT_EQ(555, url.port());
    ASSERT_STREQ("/a/path/", url.path().c_str());
    ASSERT_STREQ("", url.query().c_str());
    ASSERT_STREQ("fragment", url.fragment().c_str());
}
