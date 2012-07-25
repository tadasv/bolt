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

#include "core/timeout_manager.h"

namespace bolt {
namespace core {

typedef struct timer_data_ {
    TimeoutManager *timeout_manager;
    timeout_callback_t callback;
    void *data;
} timer_data;

static void timeout_wrapper(struct ev_loop *loop, ev_timer *timer, int events)
{
    timer_data *data = static_cast<timer_data*>(timer->data);
    bool success = data->callback(data->data);

    if (success) {
    }

    TimeoutManager *tm = data->timeout_manager;
    tm->remove_timer(timer);
}

TimeoutManager::TimeoutManager(struct ev_loop *loop)
{
    loop_ = loop;
}

TimeoutManager::~TimeoutManager()
{
    timer_set_t::iterator iter;
    for (iter = timers_.begin(); iter != timers_.end(); ++iter) {
        ev_timer_stop(loop_, *iter);
        delete *iter;
    }
}

bool TimeoutManager::create_timer(ev_tstamp timeout, void *caller_data, timeout_callback_t callback)
{
    ev_timer *timer = new ev_timer;
    ev_timer_init(timer, timeout_wrapper, timeout, 0.0);

    timer_data *data = new timer_data;
    data->timeout_manager = this;
    data->callback = callback;
    data->data = caller_data;
    timer->data = data;
    timers_.insert(timer);

    ev_timer_start(loop_, timer);

    return true;
}


bool TimeoutManager::remove_timer(struct ev_timer *timer)
{
    timer_set_t::iterator iter = timers_.find(timer);

    if (iter == timers_.end()) {
        return false;
    }

    ev_timer_stop(loop_, timer);
    delete (timer_data*)timer->data;
    delete timer;
    timers_.erase(iter);

    return true;
}

}; // namespace core
}; // namespace bolt
