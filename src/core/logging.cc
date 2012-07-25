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

#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#include "logging.h"

namespace bolt {
namespace core {

static void log_vwrite(const char *level, const char *function, int line, const char *fmt, va_list args)
{
    time_t seconds;
    struct tm *time_data;
    char time_str[21];

    time(&seconds);
    time_data = localtime(&seconds);

    if (strftime(time_str, 20, "%Y-%m-%d %X", time_data) == 0) {
        time_str[0] = '\0';
    }

    printf("%s %s in %s():%d ", time_str, level, function, line);
    vprintf(fmt, args);
    printf("\n");
}


void log_write(const char *level, const char *function, int line, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log_vwrite(level, function, line, fmt, args);
    va_end(args);
}

}; // namespace core
}; // namespace bolt
