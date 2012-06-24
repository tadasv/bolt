#include <gtest/gtest.h>

#include "db/document.h"

TEST(DBDocumentTestCase, UUIDTest)
{
    uuid_t id;
    bolt::db::generate_uuid(id);

    std::string str = bolt::db::generate_uuid_str();

    ASSERT_TRUE(bolt::db::str_to_uuid(str.c_str(), id));
    ASSERT_FALSE(bolt::db::str_to_uuid("213", id));
}


TEST(DBDocumentTestCase, toStringTest)
{
    bolt::db::Document d;
    std::string str;

    ASSERT_FALSE(d.to_string(str));

    ASSERT_EQ(bolt::db::kDocumentOK, d.parse("{}"));
    ASSERT_TRUE(str.empty());
    ASSERT_TRUE(d.to_string(str));
    ASSERT_FALSE(str.empty());
}


TEST(DBDocumentTestCase, ParserTest)
{
    bolt::db::Document d;

    ASSERT_EQ(bolt::db::kDocumentOK, d.parse("{}"));
    ASSERT_EQ(bolt::db::kDocumentOK, d.parse("{\"_id\":\"123\"}"));
    ASSERT_STREQ("123", d.id().c_str());
    ASSERT_EQ(bolt::db::kDocumentOK, d.parse("{\"_id\":\"123\", \"_ttl\": 0}"));
    ASSERT_STREQ("123", d.id().c_str());
    ASSERT_EQ(bolt::db::kDocumentOK, d.parse("{\"_id\":\"123\", \"_ttl\": 1}"));
    ASSERT_STREQ("123", d.id().c_str());
    ASSERT_EQ(bolt::db::kDocumentOK, d.parse("{\"_id\":\"123\", \"_ttl\": -1}"));
    ASSERT_STREQ("123", d.id().c_str());
    ASSERT_EQ(bolt::db::kDocumentOK, d.parse("{\"_id\":\"123\", \"_ttl\": -1, \"b\": 123}"));
    ASSERT_STREQ("123", d.id().c_str());

    ASSERT_EQ(bolt::db::kDocumentErrorParseFailed, d.parse(""));
    ASSERT_EQ(bolt::db::kDocumentErrorParseFailed, d.parse("\"\""));
    ASSERT_EQ(bolt::db::kDocumentErrorParseFailed, d.parse("abc"));
    ASSERT_EQ(bolt::db::kDocumentErrorParseFailed, d.parse("123"));
    ASSERT_EQ(bolt::db::kDocumentErrorParseFailed, d.parse("1.2"));
    ASSERT_EQ(bolt::db::kDocumentErrorParseFailed, d.parse("true"));
    ASSERT_EQ(bolt::db::kDocumentErrorParseFailed, d.parse("false"));
    ASSERT_EQ(bolt::db::kDocumentErrorObjectExpected, d.parse("[]"));

    ASSERT_EQ(bolt::db::kDocumentErrorInvalidID, d.parse("{\"_id\": 123}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidID, d.parse("{\"_id\": 1.2}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidID, d.parse("{\"_id\": []}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidID, d.parse("{\"_id\": {}}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidID, d.parse("{\"_id\": true}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidID, d.parse("{\"_id\": false}"));

    ASSERT_EQ(bolt::db::kDocumentErrorInvalidTTL, d.parse("{\"_id\": \"id\", \"_ttl\": -2}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidTTL, d.parse("{\"_id\": \"id\", \"_ttl\": 0.0}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidTTL, d.parse("{\"_id\": \"id\", \"_ttl\": 1.2}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidTTL, d.parse("{\"_id\": \"id\", \"_ttl\": []}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidTTL, d.parse("{\"_id\": \"id\", \"_ttl\": {}}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidTTL, d.parse("{\"_id\": \"id\", \"_ttl\": true}"));
    ASSERT_EQ(bolt::db::kDocumentErrorInvalidTTL, d.parse("{\"_id\": \"id\", \"_ttl\": false}"));

}
