#ifndef _UTIL_H
#define _UTIL_H

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <stdbool.h>

void __usage_error(const char* msg, char* argv_0);

bool DllPathIsValid(TCHAR full_path[260]);

#endif /* _UTIL_H */
