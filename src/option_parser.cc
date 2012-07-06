#include "option_parser.h"

#include <algorithm>

namespace bolt {
namespace core {

char *get_option(char **begin, char **end, const std::string &option)
{
    char **iter = std::find(begin, end, option);
    if (iter != end && ++iter != end) {
        return *iter;
    }

    return NULL;
}


bool option_exists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

}; // namespace core
}; // namespace bolt
