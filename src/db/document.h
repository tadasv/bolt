#ifndef __BOLT_DB_DOCUMENT_H__

#define __BOLT_DB_DOCUMENT_H__

#include <uuid/uuid.h>

#include <string>

namespace bolt {
namespace db {

void generate_uuid(uuid_t id);
std::string generate_uuid_str();
std::string uuid_to_str(uuid_t id);
bool str_to_uuid(const char *str, uuid_t id);

}; // namespace db
}; // namespace bolt

#endif // end of include guard: __BOLT_DB_DOCUMENT_H__
