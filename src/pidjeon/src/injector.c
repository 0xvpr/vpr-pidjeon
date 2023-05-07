#include "definitions.h"
#include "loadlibrary.h"
#include "manualmap.h"
#include "injector.h"
#include "parser.h"
#include "util.h"
#include "mem.h"
#include "crt.h"

#include <stdio.h>

void InjectPayload(Resource* restrict data, Injector* restrict pInjector)
{
    fprintf(stdout, "Searching for %s...\n", data->target_process);
    while (!data->process_id)
    {
        data->process_id = GetProcessIdByProcessName(data->target_process);
        Sleep(100);
    }
    fprintf(stdout, "%s Found.\n\n", data->target_process);

    if (pInjector->delay_ms)
    {
        printf("Delay(ms): %d\n\n", pInjector->delay_ms);
        Sleep(pInjector->delay_ms);
    }

    switch (pInjector->operation)
    {
        case INJECT_LOAD_LIBRARY_A:
        {
            pInjector->status = inject_LoadLibraryA((DWORD)data->process_id, data->dll_rel_path);
            break;
        }
        case INJECT_LOAD_LIBRARY_W:
        {
            pInjector->status = inject_LoadLibraryW((DWORD)data->process_id, data->dll_rel_path);
            break;
        }
        case INJECT_MANUAL_MAP:
        {
            pInjector->status = inject_ManualMap((DWORD)data->process_id, data->dll_rel_path);
            break;
        }
        case INJECT_CRT:
        {
            pInjector->status = InjectExecuteShellcode(data, pInjector);
            break;
        }
        default:
        {
            break;
        }
    }

    if (pInjector->status != 0)
    {
        __handle_error(pInjector->status);
    }
    printf("Injection: %s.\n", (pInjector->status ? "Failed" : "Successful"));
}
