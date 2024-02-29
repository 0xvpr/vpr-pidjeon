#include "logger.h"

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

#include <stdio.h>


static inline int is_valid_dll(const char* restrict path)
{
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        return 0;
    }

    return 1;
}

unsigned inject(DWORD process_id, char* restrict dll) 
{
    if (!process_id) 
    {
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH] = { 0 };
    GetFullPathName(dll, MAX_PATH, full_dll_path, NULL);

    if (!(is_valid_dll(dll)) || !is_valid_dll(full_dll_path))
    {
        return DLL_DOES_NOT_EXIST;
    }

    LPVOID pLoadLibraryA = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibraryA)
    {
        return INJECTION_FAILED;
    }
    /*fprintf(stdout, "LoadLibraryA: %p\n", pLoadLibraryA);*/

    HANDLE process_handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, process_id);
    if (!process_handle)
    {
        return INJECTION_FAILED;
    }
    /*fprintf(stdout, "process_handle: %p\n", process_handle);*/

    // Allocate space to write the dll function
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    /*fprintf(stdout, "dll_parameter_address: %p\n", dll_parameter_address);*/

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
    /*fprintf(stdout, "dll_thread_handle: %p\n", dll_thread_handle);*/

    VirtualFreeEx(process_handle, dll_parameter_address, (size_t)wrote_memory, MEM_RELEASE);
    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}

unsigned inject_i686(DWORD process_id, char* restrict dll)
{
    if (!process_id) 
    {
        return PROCESS_NOT_RUNNING;
    }

    char full_dll_path[MAX_PATH] = { 0 };
    GetFullPathName(dll, MAX_PATH, full_dll_path, NULL);

    if (!(is_valid_dll(dll)) || !is_valid_dll(full_dll_path))
    {
        return DLL_DOES_NOT_EXIST;
    }


    HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, process_id);
    if (!process_handle)
    {
        return INJECTION_FAILED;
    }

    DWORD remote_kernel_base = 0;
    HMODULE hMods[1024] = { 0 };
    DWORD cbNeeded = 0;
    if (EnumProcessModulesEx(process_handle, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_32BIT))
    {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            char szModName[MAX_PATH] = { 0 };
            if (GetModuleFileNameEx(process_handle, hMods[i], szModName, sizeof(szModName)))
            {
                // Check if this is kernel32.dll
                if (_stricmp(szModName, "C:\\WINDOWS\\System32\\KERNEL32.DLL") == 0)
                {
                    remote_kernel_base = (DWORD)( ((uintptr_t)hMods[i]) & 0xFFFFFFFF );
                    break;
                }
            }
        }
    }

    DWORD offset = 0x30F30;
    HANDLE pLoadLibraryA = (HANDLE)(0xFFFFFFFFF & (remote_kernel_base + offset) );

    fprintf(stdout, "remote_kernel_base: %lx\n", remote_kernel_base);
    fprintf(stdout, "offset: %lx\n", offset);
    fprintf(stdout, "pLoadLibraryA: %p\n", pLoadLibraryA);

    if (!pLoadLibraryA)
    {
        return METHOD_NOT_FOUND;
    }

    // Allocate space to write the dll function
    HANDLE dll_parameter_address = VirtualAllocEx(process_handle, 0, strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    BOOL wrote_memory = WriteProcessMemory(process_handle, (PVOID)dll_parameter_address, full_dll_path, strlen(full_dll_path), NULL);
    if (!wrote_memory)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, (PVOID)dll_parameter_address, 0, NULL);
    if (!dll_thread_handle)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    VirtualFreeEx(process_handle, dll_parameter_address, (size_t)wrote_memory, MEM_RELEASE);
    CloseHandle((HANDLE)dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}

unsigned inject_wide(DWORD process_id, char* restrict dll)
{
    if (process_id == 0)
    {
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH] = { 0 };
    GetFullPathName(dll, MAX_PATH, full_dll_path, NULL);

    if (!(is_valid_dll(dll)) || !is_valid_dll(full_dll_path))
    {
        return DLL_DOES_NOT_EXIST;
    }

    WCHAR dll_w[MAX_PATH] = { 0 };
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

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryW, dll_parameter_address, 0, NULL);
    if (!dll_thread_handle)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    VirtualFreeEx(process_handle, dll_parameter_address, (size_t)wrote_memory, MEM_RELEASE);
    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}
