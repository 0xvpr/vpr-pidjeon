#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include "definitions.h"

#include <inttypes.h>

#define LOG_MSG(injector, msg, shiftwidth) \
    injector->logger && injector->logger(injector, msg, shiftwidth)

typedef int (* logger_t)(const char* restrict);

int log_basic(const Injector* injector, const char* restrict event, uint32_t shiftwidth);
int log_advanced(const Injector* injector, const char* restrict event, uint32_t shiftwidth);

#endif /* LOGGER_HEADER */
