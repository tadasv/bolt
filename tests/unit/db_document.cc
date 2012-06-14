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
