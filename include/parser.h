#ifndef _PARSER_H
#define _PARSER_H

#include "injector.h"
#include "data.h"

#define INJECT_LOAD_LIBRARY_A 0x00000001
#define INJECT_MANUAL_MAP     0x00000002
#define INJECT_ADD_DELAY      0x00000004

int ParseCommandLine(int argc, char** argv, Injector* pInjector, InjectData* pData);

#endif /* _PARSER_H */
