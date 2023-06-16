#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include "definitions.h"
#include <inttypes.h>

int LogEvent(Injector* injector, const char* restrict event, unsigned shiftwidth);

#endif /* LOGGER_HEADER */
