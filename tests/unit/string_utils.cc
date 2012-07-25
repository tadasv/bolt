#include <gtest/gtest.h>

#include "core/string_utils.h"

using namespace bolt::util::string;

TEST(StringUtilsTestCase, StringCraeteTokensTest)
{
    string_tokens_t tokens;
    string_offset_t off;

    tokens = string_get_tokens(NULL, '/');
    ASSERT_EQ(0, tokens.size());

    const char *str = "string";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(1, tokens.size());
    off = tokens[0];
    ASSERT_EQ(0, off.first);
    ASSERT_EQ(strlen(str), off.second);

    str = "";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(0, tokens.size());

    str = "/string";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(1, tokens.size());
    off = tokens[0];
    ASSERT_EQ(1, off.first);
    ASSERT_EQ(strlen(str) - 1, off.second);

    str = "/a/b";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(2, tokens.size());
    off = tokens[0];
    ASSERT_EQ(1, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[1];
    ASSERT_EQ(3, off.first);
    ASSERT_EQ(1, off.second);

    str = "/a/b/c";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(3, tokens.size());
    off = tokens[0];
    ASSERT_EQ(1, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[1];
    ASSERT_EQ(3, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[2];
    ASSERT_EQ(5, off.first);
    ASSERT_EQ(1, off.second);

    str = "/a/b/c/";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(3, tokens.size());
    off = tokens[0];
    ASSERT_EQ(1, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[1];
    ASSERT_EQ(3, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[2];
    ASSERT_EQ(5, off.first);
    ASSERT_EQ(1, off.second);

    str = "/";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(0, tokens.size());

    str = "//";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(0, tokens.size());

    str = "///";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(0, tokens.size());

    str = "////";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(0, tokens.size());

    str = "a/";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(1, tokens.size());
    off = tokens[0];
    ASSERT_EQ(0, off.first);
    ASSERT_EQ(1, off.second);

    str = "a/b";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(2, tokens.size());
    off = tokens[0];
    ASSERT_EQ(0, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[1];
    ASSERT_EQ(2, off.first);
    ASSERT_EQ(1, off.second);

    str = "a/b/";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(2, tokens.size());
    off = tokens[0];
    ASSERT_EQ(0, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[1];
    ASSERT_EQ(2, off.first);
    ASSERT_EQ(1, off.second);

    str = "a/b//";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(2, tokens.size());
    off = tokens[0];
    ASSERT_EQ(0, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[1];
    ASSERT_EQ(2, off.first);
    ASSERT_EQ(1, off.second);

    str = "//a/b//";
    tokens = string_get_tokens(str, '/');
    ASSERT_EQ(2, tokens.size());
    off = tokens[0];
    ASSERT_EQ(2, off.first);
    ASSERT_EQ(1, off.second);
    off = tokens[1];
    ASSERT_EQ(4, off.first);
    ASSERT_EQ(1, off.second);
}
