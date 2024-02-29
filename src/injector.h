#ifndef INJECTOR_HEADER
#define INJECTOR_HEADER

#include "definitions.h"

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#include <tlhelp32.h>
#endif /* VC_EXTRA_LEAN */

DWORD GetProcessIdByProcessName(const char* restrict process_name);

unsigned inject_ManualMap(DWORD process_id, const char* restrict dll_path);

void InjectPayload(Resource* pResource, Injector* restrict pInjector);

void __handle_error(unsigned error_code);

#endif /* INJECTOR_HEADER */
