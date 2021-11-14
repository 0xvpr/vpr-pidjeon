#ifndef _INJECTOR_H
#define _INJECTOR_H

#include "definitions.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#endif

DWORD GetProcessIdByProcessName(const char* process_name);

int inject_ManualMap(DWORD process_id, const char* dll_path);

void InjectPayload(Resource* pResource, Injector* pInjector);

void __handle_error(int error_code);

#endif /* _INJECTOR_H */
