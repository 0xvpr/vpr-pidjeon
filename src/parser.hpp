#ifndef   PARSER_HEADER
#define   PARSER_HEADER

#include  "definitions.hpp"

namespace types {

enum error_codes : uint32_t {
    no_error,
    missing_arguments,
    invalid_arguments,
    unsupported_arguments,
    parse_error
};

} // namespace types

namespace parser {

class argument_parser {
public:
    argument_parser() = delete;
    argument_parser(const argument_parser&) = delete;
    argument_parser& operator = (const argument_parser&) = delete;
    argument_parser(const argument_parser&&) = delete;
    argument_parser& operator = (const argument_parser&&) = delete;

    argument_parser(int argc, char** argv);

    const types::parsed_args_t& parsed_args() const;
    types::error_codes parse();
    [[noreturn]] void usage(const std::string error_message) const;
private:
    types::error_codes parse_option(const std::string& option, const std::string& value);
    types::error_codes set_flag(const std::string& flag);

    int                     argc_;
    char**                  argv_;
    types::parsed_args_t    parsed_args_;
};

} // namespace parser

#endif // PARSER_HEADER
