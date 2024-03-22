#ifndef UTIL_HEADER
#define UTIL_HEADER

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#endif // VC_EXTRA_LEAN

DWORD get_process_id_by_process_name(const char* const restrict process_name);

int is_dll_path_valid(const char* const restrict full_path);

int get_architecture(const HANDLE restrict hProcess);

#endif /* UTIL_HEADER */
