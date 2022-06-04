#ifndef MEM_HEADER
#define MEM_HEADER

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#endif /* VC_EXTRA_LEAN */

DWORD GetProcessIdByProcessName(const char* restrict process_name);

#endif /* MEM_HEADER */
