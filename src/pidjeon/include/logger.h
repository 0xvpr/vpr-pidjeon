#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include "definitions.h"
#include <inttypes.h>

char* InsertSpacing(unsigned shiftwidth, char* buffer, size_t size);

int LogEvent(Injector* injector, const char* restrict event, unsigned shiftwidth);

#endif /* LOGGER_HEADER */
