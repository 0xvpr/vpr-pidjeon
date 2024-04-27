#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include "definitions.hpp"

#include <cinttypes>

#define LOG_MSG(injector, msg, shiftwidth) \
    injector.logger && injector.logger(injector, msg, shiftwidth)

typedef int (* logger_t)(const types::injector&, const char*, std::uint32_t);

int log_basic(const types::injector& injector, const char* event, std::uint32_t shiftwidth);
int log_advanced(const types::injector& injector, const char* event, std::uint32_t shiftwidth);

constexpr logger_t loggers[3] = {
    nullptr,
    log_basic,
    log_advanced
};

#endif // LOGGER_HEADER
