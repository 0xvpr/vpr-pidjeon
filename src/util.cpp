#include "util.hpp"

#include "definitions.hpp"
#include "pe32.hpp"

#include <tlhelp32.h>

typedef BOOL (WINAPI * IsWow64Process2_t)(HANDLE, USHORT*, USHORT*);

std::vector<DWORD> get_process_id_by_process_name(const char* const process_name) {
    std::vector<DWORD> processes;

    PROCESSENTRY32 process_entry{};
    process_entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE processes_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(processes_snapshot, &process_entry))
    {
        do
        {
            if (strcmp(process_entry.szExeFile, process_name) == 0)
            {
                processes.push_back(process_entry.th32ProcessID);
            }
        } while (Process32Next(processes_snapshot, &process_entry));
    }

    CloseHandle(processes_snapshot);
    return processes;
}

std::string get_process_name_by_process_id(DWORD process_id) {
    PROCESSENTRY32 process_entry{};
    process_entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE processes_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(processes_snapshot, &process_entry))
    {
        do
        {
            if (process_entry.th32ProcessID == process_id)
            {
                CloseHandle(processes_snapshot);
                return process_entry.szExeFile;
            }
        } while (Process32Next(processes_snapshot, &process_entry));
    }

    CloseHandle(processes_snapshot);
    return "";
}

int get_architecture(const HANDLE process_handle) {
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
        return types::machine_type_err;
    }

    if ((process_machine) & IMAGE_FILE_32BIT_MACHINE)
    {
        fprintf(stdout, "PE32 detected\n");
        return types::machine_x86;
    }
    else
    {
        fprintf(stdout, "PE32+ detected\n");
        return types::machine_x64;
    }

    return types::machine_unknown;
}
