#include "util.h"
#include "pe32.h"
#include "definitions.h"

#include <stdlib.h>
#include <stdio.h>

typedef BOOL (WINAPI * IsWow64Process2_t)(HANDLE, USHORT*, USHORT*);
IsWow64Process2_t fIsWow64Process2;

void __usage_error(const char* restrict msg, char* argv_0)
{
    fprintf(stderr,
        "Error message: %s.\n"
        "\n"
        "Usage: %s <target_process> <path/to/payload> [ <optional arguments> ]\n"
        "\n"
        "positional arguments:\n"
        "  target_process.exe, path/to/payload\n"
        "\n"
        "optional arguments:\n"
        "  -d\t\tadd delay to the injection (milliseconds)\n"
        "  -i\t\tspecify injection method\n"
        "  -o,--output\tspecify output file\n"
        "  -r\t\tcreate remote thread\n"
        "  -s,--silent\tsuppress output\n"
        "  -S,--stealth\tspecify stealth level\n"
        "  -V\t\tspecify verbosity level\n"
        "\n"
        "example:\n"
        "  %s calc.exe ./payload.dll -i LoadLibraryA -d 2000\n"
        , msg, argv_0, argv_0
    );

    exit(1);
}

void __handle_error(unsigned inject_code)
{
    switch (inject_code)
    {
        case INCORRECT_PARAMETERS:
            fprintf(stdout, "Error code %d: Incorrect Paramters.\n", INCORRECT_PARAMETERS);
            break;
        case PROCESS_NOT_RUNNING:
            fprintf(stdout, "Error code %d: Process is not running.\n", PROCESS_NOT_RUNNING);
            break;
        case DLL_DOES_NOT_EXIST:
            fprintf(stdout, "Error code %d: DLL does not exist.\n", DLL_DOES_NOT_EXIST);
            break;
        case INJECTION_FAILED:
            fprintf(stdout, "Error code %d: Injection Failed.\n", DLL_DOES_NOT_EXIST);
            break;
        default:
            break;
    }
}

int DllPathIsValid(char* restrict full_path)
{
    char path_buffer[MAX_PATH] = { 0 };
    for (unsigned i = 0; i < MAX_PATH; ++i)
    {
        path_buffer[i] = full_path[i];
    }

    FILE* fp;
    if (!(fp = fopen( path_buffer, "rb")))
    {
        fprintf(stderr, "invalid path: %s?\n", path_buffer);
        return 0;
    }

    fclose(fp);
    return 1;
}

int GetArchitechture(HANDLE hProcess)
{
    unsigned short pHostMachine;
    unsigned short pProcessMachine;

    fIsWow64Process2 = (IsWow64Process2_t)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process2");
    if (!fIsWow64Process2)
    {
        fprintf(stderr, "Failed to load IsWow64Process2\n");
        return -1;
    }
    else
    {
        fprintf(stdout, "fIsWow64Process2 found: %p\n", (void *)fIsWow64Process2);
    }

    if (!fIsWow64Process2(hProcess, &pProcessMachine, &pHostMachine))
    {
        fprintf(stderr, "Failed to detect architecture\n");
        return -1;
    }

    if ((pProcessMachine) & IMAGE_FILE_32BIT_MACHINE)
    {
        fprintf(stdout, "PE32 detected\n");
        return 1;
    }
    else
    {
        fprintf(stdout, "PE32+ detected\n");
        return 2;
    }

    return 0;
}
