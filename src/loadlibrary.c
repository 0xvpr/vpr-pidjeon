#include "definitions.h"
#include "loadlibrary.h"
#include "util.h"

int inject_LoadLibraryA(DWORD process_id, const char* dll)
{
    if (process_id == 0)
        return PROCESS_NOT_RUNNING;

    TCHAR full_dll_path[MAX_PATH];
    GetFullPathName(dll, MAX_PATH, full_dll_path, NULL);

    if (DllPathIsValid(full_dll_path) != 0)
        return DLL_DOES_NOT_EXIST;

    LPVOID load_library = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (load_library == NULL)
        return INJECTION_FAILED;

    HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, process_id);
    if (process_handle == NULL)
        return INJECTION_FAILED;

    // Allocate space to write the dll function
    LPVOID dll_parameter_address = VirtualAllocEx(process_handle, 0, strlen(full_dll_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (dll_parameter_address == NULL)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    BOOL wrote_memory = WriteProcessMemory(process_handle, dll_parameter_address, full_dll_path, strlen(full_dll_path), NULL);
    if (wrote_memory == false)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    HANDLE dll_thread_handle = CreateRemoteThread(process_handle, 0, 0, (LPTHREAD_START_ROUTINE)load_library, dll_parameter_address, 0, 0);
    if (dll_thread_handle == NULL)
    {
        CloseHandle(process_handle);
        return INJECTION_FAILED;
    }

    CloseHandle(dll_thread_handle);
    CloseHandle(process_handle);
    
    return 0;
}
