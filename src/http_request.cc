#include "logging.h"
#include "http_request.h"

namespace bolt {
namespace network {
namespace http {

static int on_message_begin(http_parser *parser);
static int on_uri(http_parser *parser, const char *at, size_t len);
static int on_header_field(http_parser *parser, const char *at, size_t len);
static int on_header_value(http_parser *parser, const char *at, size_t len);
static int on_headers_complete(http_parser *parser);
static int on_body(http_parser *parser, const char *at, size_t len);
static int on_message_complete(http_parser *parser);

static const http_parser_settings kParserSettings = {
    on_message_begin,
    on_uri,
    on_header_field,
    on_header_value,
    on_headers_complete,
    on_body,
    on_message_complete
};


Request::Request()
    : body_len_(0), body_len_unknown_(true),
      parse_finished_(false)
{
    http_parser_init(&parser_, HTTP_REQUEST);
    parser_.data = this;
}


Request::~Request()
{
}


size_t Request::parse(const char *data, const size_t &len)
{
    size_t nparsed = http_parser_execute(&parser_, &kParserSettings, data, len);

    if (nparsed != len) {
        // TODO bad request
    }

    return nparsed;
}


void Request::set_uri(const char *data, const size_t &len)
{
    if (!data || len == 0) {
        uri_.clear();
        return;
    }

    uri_ = std::string(data, len);
}


void Request::set_body(const char *data, const size_t &len)
{
    if (!data || len == 0) {
        body_.clear();
        return;
    }

    body_ = std::string(data, len);
}


void Request::set_header_field(const char *data, const size_t &len)
{
    if (!data || len == 0) {
        header_field_.clear();
        return;
    }

    header_field_ = std::string(data, len);
}


void Request::set_header_value(const char *data, const size_t &len)
{
    if (!data || len == 0) {
        header_value_.clear();
        return;
    }

    header_value_ = std::string(data, len);
}

void Request::update_body(const char *data, const size_t &len)
{
    if (!data || len == 0) {
        return;
    }

    body_ += std::string(data, len);
}

void Request::update_header_field(const char *data, const size_t &len)
{
    if (!data || len == 0) {
        return;
    }

    header_field_ += std::string(data, len);
}

void Request::update_header_value(const char *data, const size_t &len)
{
    if (!data || len == 0) {
        return;
    }

    header_value_ += std::string(data, len);
}


std::string Request::header_field() const
{
    return header_field_;
}


std::string Request::header_value() const
{
    return header_value_;
}


void Request::add_header(const std::string &header, const std::string &value)
{
    headers_[header] = value;
}

void Request::on_message_complete()
{
    parse_finished_ = true;
}

static int on_message_begin(http_parser *parser)
{
    return 0;
}


static int on_uri(http_parser *parser, const char *at, size_t len)
{
    if (len == 0) {
        return -1;
    }

    Request *request = static_cast<Request*>(parser->data);
    request->set_uri(at, len);

    return 0;
}


static int on_header_field(http_parser *parser, const char *at, size_t len)
{
    Request *request = static_cast<Request*>(parser->data);

    if (request->has_header_value()) {
        request->add_header(request->header_field(), request->header_value());
        request->set_header_value(NULL, 0);
        request->set_header_field(at, len);
    } else {
        request->update_header_field(at, len);
    }

    return 0;
}


static int on_header_value(http_parser *parser, const char *at, size_t len)
{
    Request *request = static_cast<Request*>(parser->data);

    if (request->has_header_value()) {
        request->update_header_value(at, len);
    } else {
        request->set_header_value(at, len);
    }

    return 0;
}


static int on_headers_complete(http_parser *parser)
{
    return 0;
}


static int on_body(http_parser *parser, const char *at, size_t len)
{
    Request *request = static_cast<Request*>(parser->data);

    if (request->has_body()) {
        request->update_body(at, len);
    } else {
        request->set_body(at, len);
    }

    return 0;
}


static int on_message_complete(http_parser *parser)
{
    Request *request = static_cast<Request*>(parser->data);

    request->on_message_complete();
    return 0;
}

}; // namesapce http
}; // namespace network
}; // namespace bolt
