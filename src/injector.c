#include "definitions.h"
#include "loadlibrary.h"
#include "manualmap.h"
#include "injector.h"
#include "parser.h"
#include "util.h"
#include <stdio.h>

DWORD GetProcessIdByProcessName(const char* process_name)
{
    PROCESSENTRY32 process_entry = { 0 };
    process_entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE processes_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(processes_snapshot, &process_entry))
    {
        do
        {
            if (strcmp(process_entry.szExeFile, process_name) == 0)
            {
                CloseHandle(processes_snapshot);
                return process_entry.th32ProcessID;
            } } while (Process32Next(processes_snapshot, &process_entry));
    }

    CloseHandle(processes_snapshot);
    return 0;
}

void InjectPayload(InjectData* data, Injector* injector)
{
    // short verbosity = injector->verbosity;
    // (void)verbosity;

    printf("Searching for %s...\n", data->target_process);
    while (!data->process_id)
    {
        data->process_id = GetProcessIdByProcessName(data->target_process);
    }
    printf("%s Found.\n\n", data->target_process);

    if (injector->delay_ms)
    {
        printf("Delay(ms): %d\n\n", injector->delay_ms);
        Sleep(injector->delay_ms);
    }

    if (injector->operation & INJECT_LOAD_LIBRARY_A)
    {
        injector->status = inject_LoadLibraryA(data->process_id, data->dll_rel_path);
    }
    else if (injector->operation & INJECT_MANUAL_MAP)
    {
        injector->status = inject_ManualMap(data->process_id, data->dll_rel_path);
    }

    if (injector->status)
    {
        __handle_error(injector->status);
    }
    printf("Injection: %s.\n", (injector->status ? "" : "Successful"));
}
