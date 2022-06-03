#include "pe32.h"
#include "mem.h"

#include <string.h>
#include <stdio.h>

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#include <psapi.h>
#endif /* VC_EXTRA_LEAN */

// TODO: Determine if file is PE
// TODO: Check to see if vpr-pidjeon-x64.exe exists
// TODO: Check to see if vpr-pidjeon-x86.exe exists

int get_archtitecture(char* restrict file_path)
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

    if (header.msDosStub.mMagic != *(uint16_t *)"MZ")
    {
        fprintf(stderr, "File does not start with magic bits 'MZ': %s\n", file_path);
        fclose(fp);
        return 0;
    }

    if (header.pe32PlusOptionalHeader.mMagic == 0x010b)
    {
        fprintf(stdout, "%s\n", "PE32 detected.");
        fclose(fp);
        return 1;
    }

    if (header.pe32PlusOptionalHeader.mMagic == 0x020b)
    {
        fprintf(stdout, "%s\n", "PE32+ detected");
        fclose(fp);
        return 2;
    }

    fclose(fp);
    return 0;
}

int main(int argc, char** argv)
{
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFO         si = { 0 };
    BOOL                rv = 0;

    if (argc < 3)
    {
        return -1;
    }

    // TODO: DELETE ME
    {
        char buffer[MAX_PATH];
        HANDLE p = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetProcessIdByProcessName(argv[1]));
        GetModuleFileNameExA(p, NULL, buffer, MAX_PATH);
        CloseHandle(p);
        fprintf(stdout, "%s\n", buffer);
    }

    char command[1024] = { 0 };
    int arch = get_archtitecture(argv[1]);
    switch (arch)
    {
        case 0:
        {
            fprintf(stderr, "%s\n", "Not a PE32\n");
            return -1;
            break;
        }
        case 1:
        {
            fprintf(stdout, "Selecting %s.\n", "vpr-pidjeon-x86.exe");
            snprintf(command, sizeof(command), "%s", "vpr-pidjeon-x86.exe");
            break;
        }
        case 2:
        {
            fprintf(stdout, "Selecting %s.\n", "vpr-pidjeon-x64.exe");
            snprintf(command, sizeof(command), "%s", "vpr-pidjeon-x64.exe");
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
