#include "definitions.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>

void __usage_error(const char* msg, char* argv_0)
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

void __handle_error(int inject_code)
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

bool DllPathIsValid(TCHAR full_path[260])
{
    FILE* fp;
    if (fopen_s(&fp, TEXT(full_path), "r"))
        fclose(fp);
    else
        return false;

    return true;
}
