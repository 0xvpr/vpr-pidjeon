#ifndef LOAD_LIBRARY_HEADER
#define LOAD_LIBRARY_HEADER

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#endif /* VC_EXTRA_LEAN */

int inject_LoadLibraryA(DWORD process_id, const char* dll);
int inject_LoadLibraryW(DWORD process_id, const char* dll);

#endif /* LOAD_LIBRARY_HEADER */
