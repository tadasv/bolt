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

#include "db/set_manager.h"
#include "db/set.h"

namespace bolt {
namespace db {

SetManager::SetManager()
    : expiration_tm_(EV_DEFAULT)
{
}

Set* SetManager::find(const std::string &set_name) const
{
    set_map_t::const_iterator iter = sets_.find(set_name);
    if (iter == sets_.end()) {
        return NULL;
    }

    return iter->second;
}


bool SetManager::add(Set *set)
{
    if (!set) {
        return false;
    }

    if (find(set->name())) {
        return false;
    }

    sets_[set->name()] = set;
    return true;
}


const set_map_t & SetManager::sets() const
{
    return sets_;
}

}; // namespace db
}; // namespace bolt
