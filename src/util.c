#include "util.h"
#include <stdio.h>
#include <stdlib.h>

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
