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

#ifndef __BOLT_DB_SET_H__

#define __BOLT_DB_SET_H__

#include <string>
#include <map>

namespace bolt {
namespace db {

class Document;

typedef std::map<std::string, Document*> document_map_t;

class Set {
    public:
        Set(const std::string &name);

        const std::string & name() const;
        bool add(Document *document);
        Document *find(const std::string &id) const;
        size_t size() const;
    private:
        std::string name_;
        document_map_t documents_;

}; // namespace db
}; // namespace bolt

};

#endif // end of include guard: __BOLT_DB_SET_H__
