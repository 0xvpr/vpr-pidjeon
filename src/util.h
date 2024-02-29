#ifndef UTIL_HEADER
#define UTIL_HEADER

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#endif // VC_EXTRA_LEAN

void __usage_error(const char* restrict msg, char* restrict argv_0);

int DllPathIsValid(char* restrict full_path);

int GetArchitechture(HANDLE restrict hProcess);

#endif /* UTIL_HEADER */
