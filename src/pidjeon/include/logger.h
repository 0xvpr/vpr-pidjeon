#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include "definitions.h"
#include <inttypes.h>

int log_basic(const Injector* injector, const char* restrict event, uint32_t shiftwidth);
int log_advanced(const Injector* injector, const char* restrict event, uint32_t shiftwidth);

#endif /* LOGGER_HEADER */
