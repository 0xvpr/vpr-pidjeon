#ifndef   LOGGER_HEADER
#define   LOGGER_HEADER

#include  "definitions.hpp"

#ifndef   LOG_MSG
#define   LOG_MSG(injector, msg, shiftwidth) \
          injector.logger && injector.logger(injector, msg, shiftwidth)
#endif // LOG_MSG

std::int32_t log_basic(const types::parsed_args_t& args, const char * event, std::uint32_t shiftwidth);
std::int32_t log_advanced(const types::parsed_args_t& args, const char* event, std::uint32_t shiftwidth);

constexpr types::logger_t loggers[3] = {
    nullptr,
    log_basic,
    log_advanced
};

#endif // LOGGER_HEADER
