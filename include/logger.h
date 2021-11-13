#ifndef _LOGGER_H
#define _LOGGER_H

#include "definitions.h"

const char* InsertSpacing(int shiftwidth);

int LogEvent(Injector* injector, const char* event, int shiftwidth);

#endif /* _LOGGER_H */
