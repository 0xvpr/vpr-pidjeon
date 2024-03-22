#include "util.h"

#include "definitions.h"
#include "pe32.h"

#include <stdio.h>

#include <tlhelp32.h>

typedef BOOL (WINAPI * IsWow64Process2_t)(HANDLE, USHORT*, USHORT*);

DWORD get_process_id_by_process_name(const char* const process_name)
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

int is_dll_path_valid(const char* const restrict full_path)
{
    char path_buffer[MAX_PATH] = { 0 };
    memcpy(path_buffer, full_path, sizeof(path_buffer)-1);

    FILE* fp;
    if (!(fp = fopen( path_buffer, "rb")))
    {
        fprintf(stderr, "invalid path: %s?\n", path_buffer);
        return 0;
    }

    fclose(fp);
    return 1;
}

int get_architecture(const HANDLE restrict process_handle)
{
    unsigned short host_machine;
    unsigned short process_machine;

    IsWow64Process2_t fIsWow64Process2 = (IsWow64Process2_t)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process2");
    if (!fIsWow64Process2)
    {
        fprintf(stderr, "Failed to load IsWow64Process2\n");
        return -1;
    }
    else
    {
        fprintf(stdout, "fIsWow64Process2 found: %p\n", (void *)fIsWow64Process2);
    }

    if (!fIsWow64Process2(process_handle, &process_machine, &host_machine))
    {
        fprintf(stderr, "Failed to detect architecture\n");
        return machine_type_err;
    }

    if ((process_machine) & IMAGE_FILE_32BIT_MACHINE)
    {
        fprintf(stdout, "PE32 detected\n");
        return machine_x86;
    }
    else
    {
        fprintf(stdout, "PE32+ detected\n");
        return machine_x64;
    }

    return machine_unknown;
}
