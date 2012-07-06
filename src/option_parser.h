#ifndef __BOLT_OPTION_PARSER_H__

#define __BOLT_OPTION_PARSER_H__

#include <string>

namespace bolt {
namespace core {

char *get_option(char **begin, char **end, const std::string &option);
bool option_exists(char **begin, char **end, const std::string &option);

}; // namespace core
}; // namespace bolt

#endif // end of include guard: __BOLT_OPTION_PARSER_H__
