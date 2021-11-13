#ifndef _LOAD_LIBRARY_H
#define _LOAD_LIBRARY_H

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

int inject_LoadLibraryA(DWORD process_id, const char* dll);

#endif /* LOAD_LIBRARY_H */
