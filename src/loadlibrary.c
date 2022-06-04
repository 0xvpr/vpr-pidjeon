#include "definitions.h"
#include "loadlibrary.h"
#include "logger.h"
#include "util.h"

int inject_LoadLibraryA(DWORD process_id, const char* restrict dll)
{
    if (process_id == 0)
    {
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH];
    GetFullPathName(dll, MAX_PATH, full_dll_path, NULL);

    if (DllPathIsValid(full_dll_path) != 0)
    {
        return DLL_DOES_NOT_EXIST;
    }

    LPVOID pLoadLibraryA = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibraryA)
    {
        return INJECTION_FAILED;
    }

    HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (!process_handle)
    {
        return INJECTION_FAILED;
    }

    // Allocate space to write the dll function
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    BOOL wrote_memory = WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path, strlen(full_dll_path), NULL);
    if (!wrote_memory)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, 0, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, dll_parameter_address, 0, 0);
    if (!dll_thread_handle)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}

int inject_LoadLibraryW(DWORD process_id, const char* restrict dll)
{
    if (process_id == 0)
    {
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH];
    GetFullPathName(dll, MAX_PATH, full_dll_path, NULL);

    if (DllPathIsValid(full_dll_path) != 0)
    {
        return INJECTION_FAILED;
    }

    LPVOID pLoadLibraryW = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryW");
    if (!pLoadLibraryW)
    {
        return INJECTION_FAILED;
    }

    HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (!process_handle)
    {
        return INJECTION_FAILED;
    }

    // Allocate space to write the dll function
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    BOOL wrote_memory = WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path, strlen(full_dll_path), NULL);
    if (!wrote_memory)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, 0, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryW, dll_parameter_address, 0, 0);
    if (!dll_thread_handle)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}
