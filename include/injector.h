#ifndef _INJECTOR_H
#define _INJECTOR_H

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
#endif

#include "definitions.h"
#include <windows.h>
#include <tlhelp32.h>
#include <stdbool.h>

DWORD GetProcessIdByProcessName(const char* process_name);

int inject_ManualMap(DWORD process_id, const char* dll_path);

void InjectPayload(InjectData* injectData, Injector* injector);

void __handle_error(int error_code);

#endif /* _INJECTOR_H */
