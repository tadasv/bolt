#include <gtest/gtest.h>

#include "db/set.h"
#include "db/document.h"

TEST(DBSetTestCase, ConstructorTest)
{
    bolt::db::Set s("name");
    ASSERT_STREQ("name", s.name().c_str());
}


TEST(DBSetTestCase, AddTest)
{
    bolt::db::Set s("name");

    ASSERT_FALSE(s.add(NULL));

    bolt::db::Document d;
    ASSERT_TRUE(s.add(&d));
    ASSERT_FALSE(s.add(&d));

    bolt::db::Document d2;
    d2.parse("{\"_id\": \"123\"}");
    ASSERT_TRUE(s.add(&d2));
    ASSERT_FALSE(s.add(&d2));
}


TEST(DBSetTestCase, FindTest)
{
    bolt::db::Set s("name");

    ASSERT_EQ(NULL, s.find(std::string("123")));

    bolt::db::Document d;
    d.parse("{\"_id\": \"123\"}");
    s.add(&d);
    ASSERT_EQ(&d, s.find(std::string("123")));
}
