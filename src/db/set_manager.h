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

#ifndef __BOLT_DB_SET_MANAGER_H__

#define __BOLT_DB_SET_MANAGER_H__

#include <string>
#include <map>

#include "core/timer_manager.h"

namespace bolt {
namespace db {

class Set;

typedef std::map<std::string, Set*> set_map_t;

class SetManager {
    public:
        SetManager();

        bool add(Set *set);
        Set *find(const std::string &set_name) const;
        const set_map_t & sets() const;
    private:
        set_map_t sets_;
        bolt::core::TimerManager expiration_tm_;
};

}; // namespace db
}; // namespace bolt

#endif // end of include guard: __BOLT_DB_SET_MANAGER_H__
