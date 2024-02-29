#ifndef LOAD_LIBRARY_HEADER
#define LOAD_LIBRARY_HEADER

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#endif /* VC_EXTRA_LEAN */

unsigned inject(DWORD process_id, char* restrict dll);
unsigned inject_i686(DWORD process_id, char* restrict dll);
unsigned inject_wide(DWORD process_id, char* restrict dll);

#endif /* LOAD_LIBRARY_HEADER */
