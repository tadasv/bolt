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

#include "core/timer_manager.h"

namespace bolt {
namespace core {

static void timeout_wrapper(struct ev_loop *loop, ev_timer *timer, int events)
{
    timer_data_t *data = static_cast<timer_data_t*>(timer->data);
    bool success = data->callback(data->object, data->data);

    if (success) {
    }

    TimerManager *tm = data->timer_manager;
    tm->remove_timer(data->object);
}

TimerManager::TimerManager(struct ev_loop *loop)
{
    loop_ = loop;
}

TimerManager::~TimerManager()
{
    timer_map_t::iterator iter;
    for (iter = timers_.begin(); iter != timers_.end(); ++iter) {
        timer_data_t *data = iter->second;
        ev_timer_stop(loop_, data->timer);
        delete data->timer;
        delete data;
    }
}

bool TimerManager::create_timer(void *object, void *data, ev_tstamp timeout, timeout_callback_t callback)
{
    ev_timer *timer = new ev_timer;
    ev_timer_init(timer, timeout_wrapper, timeout, 0.0);

    timer_data_t *tdata = new timer_data_t;
    tdata->timer_manager = this;
    tdata->callback = callback;
    tdata->data = data;
    tdata->object = object;
    tdata->timer = timer;
    timer->data = tdata;
    timers_[object] = tdata;

    ev_timer_start(loop_, timer);
    return true;
}


bool TimerManager::remove_timer(void *object)
{
    timer_map_t::iterator iter = timers_.find(object);

    if (iter == timers_.end()) {
        return false;
    }

    timer_data_t *data = static_cast<timer_data_t*>(iter->second);
    ev_timer_stop(loop_, data->timer);
    delete data->timer;
    delete data;
    timers_.erase(iter);

    return true;
}

}; // namespace core
}; // namespace bolt
