#ifndef INJECTOR_HEADER
#define INJECTOR_HEADER

#include "definitions.hpp"
#include "util.hpp"

#include <windows.h>
#include <tlhelp32.h>

unsigned inject_ManualMap(DWORD process_id, const char* dll_path);

void inject_payload(resource& resource, injector& injector);

void __handle_error(int error_code);

#endif // INJECTOR_HEADER
