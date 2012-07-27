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

#ifndef __BOLT_CORE_TIMER_MANAGER__

#define __BOLT_CORE_TIMER_MANAGER__

#include <map>

#include <ev.h>

namespace bolt {
namespace core {

typedef bool(*timeout_callback_t)(void *object, void *data);

class TimerManager;

typedef struct timer_data_ {
    TimerManager *timer_manager;
    struct ev_timer *timer;
    timeout_callback_t callback;
    void *object;
    void *data;
} timer_data_t;

typedef std::map<void *, timer_data_t *> timer_map_t;

class TimerManager {
    public:
        TimerManager(struct ev_loop *loop);
        ~TimerManager();

        bool create_timer(void *object, void *data, ev_tstamp timeout, timeout_callback_t callback);
        bool remove_timer(void *object);
    private:
        struct ev_loop *loop_;
        timer_map_t timers_;
};

}; // namespace core
}; // namespace bolt

#endif // end of include guard: __BOLT_CORE_TIMER_MANAGER__
