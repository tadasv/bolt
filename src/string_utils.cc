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

#include "string_utils.h"

namespace bolt {
namespace util {
namespace string {

string_tokens_t string_get_tokens(const char *str, const char &delimiter)
{
    string_tokens_t tokens;

    if (str == NULL || *str == '\0') {
        return tokens;
    }

    const char *delim = NULL;
    const char *prev_delim = NULL;
    size_t offset = 0;
    ssize_t size = 0;

    const char *p = str;

    while (*p) {
        if (*p == delimiter) {
            delim = p;

            if (prev_delim) {
                if (prev_delim != delim) {
                    size = delim - prev_delim - 1;
                    offset = prev_delim + 1 - str;
                    if (size != 0) {
                        tokens.push_back(string_offset_t(offset, size));
                    }
                    prev_delim = delim;
                }
            } else {
                if (delim == str) {
                    prev_delim = delim;
                } else {
                    size = delim - str;
                    if (size != 0) {
                        tokens.push_back(string_offset_t(0, size));
                    }
                    prev_delim = delim;
                }
            }
        }
        p++;
    }

    size_t len = p - str;

    if (delim == NULL) {
        tokens.push_back(string_offset_t(0, len));
    } else if (prev_delim == delim) {
        offset = delim - str + 1;
        size = len - offset;
        if (size != 0) {
            tokens.push_back(string_offset_t(offset, size));
        }
    }

    return tokens;
}

}; // namespace string
}; // namespace util
}; // namespace bolt
