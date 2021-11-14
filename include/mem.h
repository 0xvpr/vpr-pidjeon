#ifndef _MEM_H
#define _MEM_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

DWORD GetProcessIdByProcessName(const char* process_name);

#endif /* _MEM_H */
