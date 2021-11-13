/**
 * Author:   VPR
 * Created:  September 14, 2021
 * Modified: September 17, 2021
**/

#include "manualmap.h"
#include "injector.h"
#include "parser.h"
#include "data.h"
#include <stdio.h>

#define DEBUG

int main(int argc, char* argv[])
{
    Injector injector = { .status      = 0,
                          .operation   = 0,
                          .delay_ms    = 0,
                          .remote      = 0,
                          .silent      = 0,
                          .stealth     = 0,
                          .verbosity   = 1,
                          .output_file = "log.txt" };

    InjectData data = { 0 };

    if (ParseCommandLine(argc, argv, &injector, &data) == -1) { return -1; }

#ifdef DEBUG
    printf("target: %s\n", data.target_process);       // DEBUG
    printf("payload: %s\n", data.dll_rel_path);        // DEBUG
    printf("output file: %s\n", injector.output_file); // DEBUG
#endif

    printf("Searching for %s...\n", data.target_process);
    while (!data.process_id)
    {
        data.process_id = GetProcessIdByProcessName(data.target_process);
    }
    printf("%s Found.\n\n", data.target_process);

#ifdef DEBUG
    printf("PID: %d\n", data.process_id);
#endif

    if (injector.delay_ms)
    {
        printf("Delay(ms): %d\n\n", injector.delay_ms);
        Sleep(injector.delay_ms);
    }

    if (injector.operation & INJECT_LOAD_LIBRARY_A)
    {
        injector.status = inject_LoadLibraryA(data.process_id, data.dll_rel_path);
    }
    else if (injector.operation & INJECT_MANUAL_MAP)
    {
        injector.status = inject_ManualMap(data.process_id, data.dll_rel_path);
    }

    if (injector.status)
    {
        __handle_error(injector.status);
    }
    printf("Injection: %s.\n", (injector.status ? "Failed" : "Successful"));

    return injector.status;
}
