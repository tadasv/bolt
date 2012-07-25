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

#ifndef __BOLT_LOGGING_H__

#define __BOLT_LOGGING_H__

namespace bolt {
namespace core {

void log_write(const char *level, const char *function, int line, const char *fmt, ...);

#define bolt_log_write(level, msg...) (bolt::core::log_write(level,__func__,__LINE__,msg))
#define bolt_log_error(msg...) (bolt_log_write("ERROR",msg))
#define bolt_log_info(msg...) (bolt_log_write("INFO",msg))
#define bolt_log_debug(msg...) (bolt_log_write("DEBUG",msg))

}; // namespace core
}; // namespace bolt

#endif // end of include guard: __BOLT_LOGGING_H__
