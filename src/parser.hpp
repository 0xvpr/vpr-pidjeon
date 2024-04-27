#ifndef PARSER_HEADER
#define PARSER_HEADER

#include "definitions.hpp"
#include "logger.hpp"

#include <filesystem>
#include <string>
#include <chrono>

namespace parser {

namespace types {
struct parsed_args_t {
    std::string                 program_name;
    std::string                 process_name;
    std::uint32_t               process_id;
    std::filesystem::path       payload_path;
    std::string                 relative_payload_path;
    inject::operation           operation;
    std::chrono::milliseconds   delay;
    std::int32_t                verbosity;
    std::int32_t                stealth;
    bool                        silent;
    std::filesystem::path       log_path;
    logger_t                    logger;
};

enum error_codes : uint32_t {
    no_error,
    missing_arguments,
    invalid_arguments,
    unsupported_arguments,
    parse_error
};
} // namespace types

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

    int                     argc;
    char**                  argv;
    types::parsed_args_t    parsed_args_;
};

} // namespace parser

#endif // PARSER_HEADER
