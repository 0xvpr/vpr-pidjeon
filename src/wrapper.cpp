#include "util.h"
#include "mem.h"
#include "util.hpp"

#include <string.h>
#include <stdio.h>

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#include <psapi.h>
#endif /* VC_EXTRA_LEAN */

// TODO: Validate process
// TODO: Validate DLL path
// TODO: Move wait loop function to here
// TODO: Check to see if vpr-pidjeon-x64.exe exists
// TODO: Check to see if vpr-pidjeon-x86.exe exists

int main(int argc, char** argv)
{
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFO         si = { 0 };
    HANDLE              p = nullptr;
    DWORD               pid = 0;
    BOOL                arch = 0;
    BOOL                rv = 0;
    char                full_path[MAX_PATH] = { 0 };
    char                command[4096] = { 0 };

    if (argc < 3)
    {
        __usage_error("Missing positional arguments", argv[0]);
    }

    // TODO: FIX ME
    {
        pid = GetProcessIdByProcessName(argv[1]);
        p = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (!p)
        {
            fprintf(stderr, "Failed to query process: %s\n", argv[1]);
            return 2;
        }

        GetModuleFileNameExA(p, nullptr, full_path, MAX_PATH);
        CloseHandle(p);
        fprintf(stdout, "%s\n", full_path);
    }

    arch = get_architecture(full_path);
    switch (arch)
    {
        case -1:
        {
            fprintf(stderr, "Failed to detect architecture: %s\n", full_path);
            return 127;
        }
        case 0:
        {
            fprintf(stderr, "%s\n", "Not a PE32\n");
            return 128;
            break;
        }
        case 1:
        {
            char injector[256] = "vpr-pidjeon-x86.exe";
            fprintf(stdout, "Selecting %s.\n", injector);
            snprintf(command, sizeof(command), "%s", injector);
            break;
        }
        case 2:
        {
            char injector[256] = "vpr-pidjeon-x64.exe";
            fprintf(stdout, "Selecting %s.\n", injector);
            snprintf(command, sizeof(command), "%s", injector);
            break;
        }
    }

    for (int i = 1; i < argc; ++i)
    {
        strcat(command, " ");
        strcat(command, argv[i]);
    }

    si.cb = sizeof(si);
    rv = CreateProcess(
        nullptr,        // No module name (use command line)
        command,        // Command line
        nullptr,        // Process handle not inheritable
        nullptr,        // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        nullptr,        // Use parent's environment block
        nullptr,        // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure
    );

    if (!rv)
    {
        fprintf(stderr, "%s %lu\n", "Wrapper Failed: 'CreateProcess' failed with error", GetLastError());
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return EXIT_SUCCESS;
}
