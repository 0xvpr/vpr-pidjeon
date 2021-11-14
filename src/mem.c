#include "mem.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <tlhelp32.h>
#endif

DWORD GetProcessIdByProcessName(const char* process_name)
{
    PROCESSENTRY32 process_entry = { 0 };
    process_entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE processes_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(processes_snapshot, &process_entry))
    {
        do
        {
            if (strcmp(process_entry.szExeFile, process_name) == 0)
            {
                CloseHandle(processes_snapshot);
                return process_entry.th32ProcessID;
            }
        } while (Process32Next(processes_snapshot, &process_entry));
    }

    CloseHandle(processes_snapshot);
    return 0;
}
