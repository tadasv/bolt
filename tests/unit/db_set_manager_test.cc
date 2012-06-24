#include <gtest/gtest.h>

#include "db/set.h"
#include "db/set_manager.h"

TEST(DBSetManagerTestCase, ConstructorTest)
{
    bolt::db::SetManager manager;
}


TEST(DBSetManagerTestCase, AddTest)
{
    bolt::db::SetManager manager;

    ASSERT_FALSE(manager.add(NULL));

    bolt::db::Set s("set");
    ASSERT_TRUE(manager.add(&s));
    ASSERT_FALSE(manager.add(&s));

    bolt::db::Set s2("set2");
    ASSERT_TRUE(manager.add(&s2));
    ASSERT_FALSE(manager.add(&s2));
}


TEST(DBSetManagerTestCase, FindTest)
{
    bolt::db::SetManager manager;

    ASSERT_EQ(NULL, manager.find(std::string("123")));

    bolt::db::Set s("123");
    manager.add(&s);
    ASSERT_EQ(&s, manager.find(std::string("123")));
}
