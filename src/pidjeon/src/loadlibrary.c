#include "definitions.h"
#include "loadlibrary.h"
#include "logger.h"
#include "util.h"

unsigned inject_LoadLibraryA(DWORD process_id, char* restrict dll)
{
    if (!process_id) 
    {
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH];
    GetFullPathName(dll, MAX_PATH, full_dll_path, NULL);

    if (!(DllPathIsValid(dll)) || !DllPathIsValid(full_dll_path))
    {
        return DLL_DOES_NOT_EXIST;
    }

    LPVOID pLoadLibraryA = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibraryA)
    {
        return INJECTION_FAILED;
    }

    HANDLE process_handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, process_id);
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

unsigned inject_LoadLibraryW(DWORD process_id, char* restrict dll)
{
    if (process_id == 0)
    {
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH];
    GetFullPathName(dll, MAX_PATH, full_dll_path, NULL);

    if (!(DllPathIsValid(dll)) || !DllPathIsValid(full_dll_path))
    {
        return DLL_DOES_NOT_EXIST;
    }

    WCHAR dll_w[MAX_PATH];
    WCHAR full_dll_path_w[MAX_PATH] = { 0 };

    for (size_t i = 0; dll[i] != 0; ++i) { dll_w[i] = (WCHAR)dll[i]; }
    GetFullPathNameW(dll_w, MAX_PATH, full_dll_path_w, NULL);

    LPVOID pLoadLibraryW = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW");
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
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, sizeof(WCHAR) * wcslen(full_dll_path_w), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    BOOL wrote_memory = WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path_w, sizeof(WCHAR) * wcslen(full_dll_path_w), NULL);
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
