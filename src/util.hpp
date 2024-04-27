#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <windows.h>

#include <cstdio>

#include <vector>
#include <string>


std::vector<DWORD> get_process_id_by_process_name(const char* process_name);

std::string get_process_name_by_process_id(DWORD process_id);

int get_architecture(const HANDLE process_handle);

__forceinline int file_exists(const char* file) {
    FILE* fp;
    if (!(fp = fopen(file, "rb")))
    {
        return 0;
    }
    
    fclose(fp);
    return 1;
}

#endif /* UTIL_HEADER */
