#ifndef UTIL_HEADER
#define UTIL_HEADER

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#endif // VC_EXTRA_LEAN

#include <stdio.h>

DWORD get_process_id_by_process_name(const char* const restrict process_name);

int get_architecture(const HANDLE restrict hProcess);

__forceinline int file_exists(const char* const restrict file)
{
    FILE* fp;
    if (!(fp = fopen(file, "rb")))
    {
        return 0;
    }
    
    fclose(fp);
    return 1;
}

#endif /* UTIL_HEADER */
