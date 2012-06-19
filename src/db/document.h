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

#ifndef __BOLT_DB_DOCUMENT_H__

#define __BOLT_DB_DOCUMENT_H__

#include <uuid/uuid.h>
#include <jansson.h>

#include <string>

namespace bolt {
namespace db {

void generate_uuid(uuid_t id);
std::string generate_uuid_str();
std::string uuid_to_str(uuid_t id);
bool str_to_uuid(const char *str, uuid_t id);

enum DocumentError {
    kDocumentOK = 0,
    kDocumentError,
    kDocumentErrorParseFailed,
    kDocumentErrorObjectExpected,
    kDocumentErrorInvalidTTL,
    kDocumentErrorInvalidID
};

class Document {
    public:
        Document();
        ~Document();

        const std::string & id() const;

        DocumentError parse(const char *str);
        DocumentError parse(const std::string &str);
    private:
        /**
         * @brief Time to live in seconds.
         *
         * The value should be normalized to range [-1, max int].
         * The value -1 means infinite lifetime.
         */
        int ttl_;

        /**
         * @brief Document id.
         */
        std::string id_;

        /**
         * @brief JSON root representing the document.
         */
        json_t *root_;
};

}; // namespace db
}; // namespace bolt

#endif // end of include guard: __BOLT_DB_DOCUMENT_H__
