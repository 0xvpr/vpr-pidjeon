#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <stdbool.h>

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#endif // VC_EXTRA_LEAN

void __usage_error(const char* restrict msg, char* restrict argv_0);

bool DllPathIsValid(TCHAR* full_path);

int GetArchitechture(char* restrict full_path);

#endif /* UTIL_HEADER */
