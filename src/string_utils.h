// Copyright Tadas Vilkeliskis.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef __BOLT_STRING_UTILS_H__

#define __BOLT_STRING_UTILS_H__

#include <sstream>
#include <vector>
#include <utility>

namespace bolt {
namespace util {
namespace string {

template<class T>
std::string t_to_string(T i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}


typedef std::pair<size_t, size_t> string_offset_t;
typedef std::vector<string_offset_t> string_tokens_t;

/**
 * @brief Tokenize string.
 *
 * This function will create a list of tokens (index and size) split by the
 * delimiter.
 *
 * Example:
 * Delimiter: '/'
 *
 * "1/2" -> [ [0, 1], [2, 1] ]
 * "//"  -> []
 *
 * @param delim delimiter
 * @param str String.
 *
 * @return A list of tokens.
 */
string_tokens_t string_get_tokens(const char *str, const char &delim);

}; // namespace string
}; // namespace util
}; // namespace bolt

#endif // end of include guard: __BOLT_STRING_UTILS_H__
