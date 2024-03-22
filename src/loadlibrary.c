#include "loadlibrary.h"
#include "logger.h"
#include "util.h"

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>


unsigned load_library_a(const Resource * const restrict resource, const Injector * const restrict injector)
{
    HANDLE process_handle = NULL;
    if (!resource->process_id || !((process_handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, resource->process_id))) )
    {
        LOG_MSG(injector, "Failed to open process", 0);
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH] = { 0 };
    GetFullPathName(resource->relative_payload_path, MAX_PATH, full_dll_path, NULL);

    if (!(file_exists(resource->relative_payload_path)) || !file_exists(full_dll_path))
    {
        LOG_MSG(injector, "DLL path is not valid", 0);
        return DLL_DOES_NOT_EXIST;
    }
    LOG_MSG(injector, "DLL path is valid", 0);

    LPVOID pLoadLibraryA = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibraryA)
    {
        LOG_MSG(injector, "Failed to load LoadLibraryA", 0);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "LoadLibraryA successfully loaded", 0);

    // Allocate space to write the DLL function
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        LOG_MSG(injector, "Failed to allocate memory to target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Allocated memory to target process", 0);

    BOOL wrote_memory = WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path, strlen(full_dll_path), NULL);
    if (!wrote_memory)
    {
        LOG_MSG(injector, "Failed to write memory to target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Wrote memory to target process", 0);

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, 0, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, dll_parameter_address, 0, 0);
    if (!dll_thread_handle)
    {
        LOG_MSG(injector, "Failed to create remote thread in target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Remote thread created in target process", 0);

    VirtualFreeEx(process_handle, dll_parameter_address, (size_t)wrote_memory, MEM_RELEASE);
    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}

unsigned load_library_a_i686(const Resource * const restrict resource, const Injector * const restrict injector)
{
    HANDLE process_handle = NULL;
    if (!resource->process_id || !((process_handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, resource->process_id))) )
    {
        LOG_MSG(injector, "Failed to open process", 0);
        return PROCESS_NOT_RUNNING;
    }

    char full_dll_path[MAX_PATH] = { 0 };
    GetFullPathName(resource->relative_payload_path, MAX_PATH, full_dll_path, NULL);

    if (!(file_exists(resource->relative_payload_path)) || !file_exists(full_dll_path))
    {
        LOG_MSG(injector, "DLL path is not valid", 0);
        return DLL_DOES_NOT_EXIST;
    }
    LOG_MSG(injector, "DLL path is valid", 0);

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

    if (!pLoadLibraryA)
    {
        LOG_MSG(injector, "Failed to find LoadLibraryA import", 0);
        return METHOD_NOT_FOUND;
    }
    LOG_MSG(injector, "LoadLibraryA import found", 0);

    // Allocate space to write the DLL function
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        LOG_MSG(injector, "Failed to allocate memory to target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Allocated memory to target process", 0);

    BOOL wrote_memory = WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path, strlen(full_dll_path), NULL);
    if (!wrote_memory)
    {
        LOG_MSG(injector, "Failed to write memory to target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Wrote memory to target process", 0);

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, 0, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, dll_parameter_address, 0, 0);
    if (!dll_thread_handle)
    {
        LOG_MSG(injector, "Failed to create remote thread in target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Remote thread created in target process", 0);

    VirtualFreeEx(process_handle, dll_parameter_address, (size_t)wrote_memory, MEM_RELEASE);
    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}

unsigned load_library_w(const Resource * const restrict resource, const Injector * const restrict injector)
{
    HANDLE process_handle = NULL;
    if (!resource->process_id || !((process_handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, resource->process_id))) )
    {
        LOG_MSG(injector, "Failed to open process", 0);
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH] = { 0 };
    GetFullPathName(resource->relative_payload_path, MAX_PATH, full_dll_path, NULL);

    if (!(file_exists(resource->relative_payload_path)) || !file_exists(full_dll_path))
    {
        return DLL_DOES_NOT_EXIST;
    }

    WCHAR dll_w[MAX_PATH] = { 0 };
    WCHAR full_dll_path_w[MAX_PATH] = { 0 };

    for (size_t i = 0; resource->relative_payload_path[i] != 0; ++i)
    {
        dll_w[i] = (WCHAR)resource->relative_payload_path[i];
    }
    GetFullPathNameW(dll_w, MAX_PATH, full_dll_path_w, NULL);

    LPVOID pLoadLibraryW = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW");
    if (!pLoadLibraryW)
    {
        LOG_MSG(injector, "Failed to load LoadLibraryW", 0);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "LoadLibraryW successfully loaded", 0);

    // Allocate space to write the DLL function
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, wcslen(full_dll_path_w), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        LOG_MSG(injector, "Failed to allocate memory to target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Allocated memory to target process", 0);

    BOOL wrote_memory = WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path, wcslen(full_dll_path_w), NULL);
    if (!wrote_memory)
    {
        LOG_MSG(injector, "Failed to write memory to target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Wrote memory to target process", 0);

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, 0, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryW, dll_parameter_address, 0, 0);
    if (!dll_thread_handle)
    {
        LOG_MSG(injector, "Failed to create remote thread in target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Remote thread created in target process", 0);

    VirtualFreeEx(process_handle, dll_parameter_address, (size_t)wrote_memory, MEM_RELEASE);
    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}

unsigned load_library_w_i686(const Resource * const restrict resource, const Injector * const restrict injector)
{
    HANDLE process_handle = NULL;
    if (!resource->process_id || !((process_handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, resource->process_id))) )
    {
        LOG_MSG(injector, "Failed to open process", 0);
        return PROCESS_NOT_RUNNING;
    }

    TCHAR full_dll_path[MAX_PATH] = { 0 };
    GetFullPathName(resource->relative_payload_path, MAX_PATH, full_dll_path, NULL);

    if (!(file_exists(resource->relative_payload_path)) || !file_exists(full_dll_path))
    {
        return DLL_DOES_NOT_EXIST;
    }

    WCHAR dll_w[MAX_PATH] = { 0 };
    WCHAR full_dll_path_w[MAX_PATH] = { 0 };

    for (size_t i = 0; resource->relative_payload_path[i] != 0; ++i)
    {
        dll_w[i] = (WCHAR)resource->relative_payload_path[i];
    }
    GetFullPathNameW(dll_w, MAX_PATH, full_dll_path_w, NULL);

    LPVOID pLoadLibraryW = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryW");
    if (!pLoadLibraryW)
    {
        LOG_MSG(injector, "Failed to load LoadLibraryW", 0);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "LoadLibraryW successfully loaded", 0);

    // Allocate space to write the DLL function
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, wcslen(full_dll_path_w), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dll_parameter_address)
    {
        LOG_MSG(injector, "Failed to allocate memory to target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Allocated memory to target process", 0);

    BOOL wrote_memory = WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path, wcslen(full_dll_path_w), NULL);
    if (!wrote_memory)
    {
        LOG_MSG(injector, "Failed to write memory to target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Wrote memory to target process", 0);

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, 0, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryW, dll_parameter_address, 0, 0);
    if (!dll_thread_handle)
    {
        LOG_MSG(injector, "Failed to create remote thread in target process", 0);
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }
    LOG_MSG(injector, "Remote thread created in target process", 0);

    VirtualFreeEx(process_handle, dll_parameter_address, (size_t)wrote_memory, MEM_RELEASE);
    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}
