#include "db/document.h"

namespace bolt {
namespace db {

void generate_uuid(uuid_t id)
{
    uuid_generate(id);
}


std::string uuid_to_str(uuid_t id)
{
    uuid_string_t str;
    uuid_unparse(id, str);
    return std::string(str);
}


std::string generate_uuid_str()
{
    uuid_t id;
    generate_uuid(id);
    return uuid_to_str(id);
}


bool str_to_uuid(const char *str, uuid_t id)
{
    return (uuid_parse(str, id) == 0);
}


}; // namespace db
}; // namespace bolt
