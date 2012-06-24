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

#include "db/set.h"
#include "db/document.h"

namespace bolt {
namespace db {

Set::Set(const std::string &name)
{
    name_ = name;
}


const std::string & Set::name() const
{
    return name_;
}


bool Set::add(Document *document)
{
    if (!document) {
        return false;
    }

    const std::string &id = document->id();
    if (documents_.find(id) != documents_.end()) {
        return false;
    }

    documents_[id] = document;
    return true;
}


Document *Set::find(const std::string &id) const
{
    document_map_t::const_iterator iter = documents_.find(id);
    if (iter == documents_.end()) {
        return NULL;
    }

    return iter->second;
}

}; // namespace db
}; // namespace bolt
