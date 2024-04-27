#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include "definitions.hpp"

#include <cinttypes>

#define LOG_MSG(injector, msg, shiftwidth) \
    injector.logger && injector.logger(injector, msg, shiftwidth)

typedef int (* logger_t)(const injector&, const char*, std::uint32_t);

int log_basic(const injector& injector, const char* event, std::uint32_t shiftwidth);
int log_advanced(const injector& injector, const char* event, std::uint32_t shiftwidth);

#endif // LOGGER_HEADER
