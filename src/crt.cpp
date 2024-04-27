#include "crt.hpp"

#include "logger.hpp"

#include <windows.h>
#include <tlhelp32.h>

#include <stdlib.h>
#include <stdio.h>

__forceinline char* extract_bytes_from_file(const char* f, size_t* size) {
    FILE* fp = nullptr;

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

unsigned create_remote_thread(const resource& res, const injector& inj) {
    exec_mem_t              exec_mem = nullptr;
    HANDLE                  process_handle = nullptr;
    HANDLE                  hThread = nullptr;
    DWORD                   old_protect = 0;
    BOOL                    bSuccess = 0;
    char*                   payload = 0;
    size_t                  size = 0;

    LOG_MSG(inj, "Opening target process..", 0);
    if (!(process_handle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, res.process_id))) {
        LOG_MSG(inj, "Failed to acquire handle to target", 0);
        CloseHandle(process_handle);
        return 1;
    }
    LOG_MSG(inj, "Process Handle to target acquired", 0);

    LOG_MSG(inj, "Allocating virtual memory to target..", 0);
    payload = extract_bytes_from_file(res.relative_payload_path, &size);
    if (!(exec_mem = (exec_mem_t)VirtualAllocEx(process_handle, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE))) {
        LOG_MSG(inj, "Failed to allocate memory to target", 0);
        printf("exec_mem: %p\n", exec_mem);
        CloseHandle(process_handle);
        return 2;
    }
    LOG_MSG(inj, "Allocation successful", 0);

    LOG_MSG(inj, "Writing payload to executable memory..", 0);
    if (!(WriteProcessMemory(process_handle, (LPVOID)exec_mem, (LPVOID)payload, size, nullptr))) {
        LOG_MSG(inj, "Failed to write payload to executable memory", 0);
        free(payload);
        CloseHandle(process_handle);
        return 3;
    }
    free(payload);
    LOG_MSG(inj, "Write success", 0);

    LOG_MSG(inj, "Changing virtual protection permissions..", 0);
    if (!(bSuccess = VirtualProtectEx(process_handle, (LPVOID)exec_mem, size, PAGE_EXECUTE_READ, &old_protect))) {
        LOG_MSG(inj, "Failed to change virtual protection permissions", 0);
        VirtualFreeEx(process_handle, exec_mem, size, MEM_RELEASE);
        CloseHandle(process_handle);
        return 4;
    }
    LOG_MSG(inj, "Protection changed", 0);

    LOG_MSG(inj, "Creating remote thread..", 0);
    if (!(hThread = CreateRemoteThread(process_handle, nullptr, 0, (LPTHREAD_START_ROUTINE)exec_mem, nullptr, 0, nullptr))) {
        LOG_MSG(inj, "Failed to create remote thread", 0);
        VirtualFreeEx(process_handle, exec_mem, size, MEM_RELEASE);
        CloseHandle(process_handle);
        return 5;
    }
    LOG_MSG(inj, "Remote thread created", 0);
    
    // Allow injecto to specify if we wait for the thread to finish
    // WaitForSingleObject(hThread, INFINITE);
    
    // LOG_MSG(inj, "Freeing allocated memory..", 0);
    // if (!VirtualFreeEx(process_handle, exec_mem, size, MEM_RELEASE))
    // {
    //     LOG_MSG(inj, "Failed to free allocated memory", 0);
    //     CloseHandle(process_handle);
    //     return 6;
    // }
    // LOG_MSG(inj, "Allocated memory released", 0);
    
    LOG_MSG(inj, "Closing handle and returning control to owner..", 0);
    CloseHandle(process_handle);
    return 0;
}
