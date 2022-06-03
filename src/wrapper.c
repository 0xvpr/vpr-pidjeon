#include "pe32.h"
#include "util.h"
#include "mem.h"

#include <string.h>
#include <stdio.h>

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#include <psapi.h>
#endif /* VC_EXTRA_LEAN */

// TODO: Validate arguments
// TODO: Check to see if vpr-pidjeon-x64.exe exists
// TODO: Check to see if vpr-pidjeon-x86.exe exists

int GetArchitechture(char* restrict file_path)
{
    FILE* fp;
    Pe32FullHeader header = { 0 };

    if (!(fp = fopen(file_path, "rb")))
    {
        fprintf(stderr, "Failed to open: %s\n", file_path);
        return -1;
    }

    fseek(fp, 0, SEEK_SET);
    if (!fread((void *)&header, sizeof(header), 1, fp))
    {
        fprintf(stderr, "Failed to read: %s\n", file_path);
        fclose(fp);
        return -1;
    }
    fclose(fp);

    if (header.msDosStub.mMagic != *(uint16_t *)"MZ")
    {
        fprintf(stderr, "File does not start with magic bits 'MZ': %s\n", file_path);
        return 0;
    }

    if (header.pe32PlusOptionalHeader.mMagic == 0x010b)
    {
        fprintf(stdout, "%s\n", "PE32 detected.");
        return 1;
    }

    if (header.pe32PlusOptionalHeader.mMagic == 0x020b)
    {
        fprintf(stdout, "%s\n", "PE32+ detected.");
        return 2;
    }

    return 0;
}

int main(int argc, char** argv)
{
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFO         si = { 0 };
    HANDLE              p = NULL;
    DWORD               pid = 0;
    BOOL                arch = 0;
    BOOL                rv = 0;
    char                full_path[MAX_PATH] = { 0 };
    char                command[4096] = { 0 };

    if (argc < 3)
    {
        __usage_error("positional arguments required", argv[0]);
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

        GetModuleFileNameExA(p, NULL, full_path, MAX_PATH);
        CloseHandle(p);
        fprintf(stdout, "%s\n", full_path);
    }

    GetArchitechture(full_path);
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
        NULL,           // No module name (use command line)
        command,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
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
