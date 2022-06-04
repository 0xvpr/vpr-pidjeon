#include "util.h"
#include "pe32.h"
#include "definitions.h"

#include <stdlib.h>
#include <stdio.h>

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
    FILE* fp;
    if (fopen_s(&fp, TEXT(full_path), "rb"))
    {
        fclose(fp);
        return 0;
    }

    return 1;
}

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
