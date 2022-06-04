#include "manualmap.h"

#ifndef VC_EXTRA_LEAN
#define VC_EXTRA_LEAN
#include <tlhelp32.h>
#endif /* VC_EXTRA_LEAN */

#include <string.h>
#include <stdint.h>
#include <stdio.h>

DWORD __stdcall LibraryLoader(LPVOID memory)
{
    loaderdata* LoaderParams = (loaderdata*)memory;
    PIMAGE_BASE_RELOCATION pIBR = LoaderParams->BaseReloc;

    uintptr_t delta = (uintptr_t)((LPBYTE)LoaderParams->ImageBase - LoaderParams->NtHeaders->OptionalHeader.ImageBase); // Calculate the delta

    while (pIBR->VirtualAddress)
    {
        if (pIBR->SizeOfBlock >= sizeof(IMAGE_BASE_RELOCATION))
        {
            int count = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
            PWORD list = (PWORD)(pIBR + 1);

            for (int i = 0; i < count; i++)
            {
                if (list[i])
                {
                    PDWORD ptr = (PDWORD)((LPBYTE)LoaderParams->ImageBase + (pIBR->VirtualAddress + (list[i] & 0xFFF)));
                    *ptr += delta;
                }
            }
        }

        pIBR = (PIMAGE_BASE_RELOCATION)((LPBYTE)pIBR + pIBR->SizeOfBlock);
    }

    PIMAGE_IMPORT_DESCRIPTOR pIID = LoaderParams->ImportDirectory;

    // Resolve DLL imports
    while (pIID->Characteristics)
    {
        PIMAGE_THUNK_DATA OrigFirstThunk = (PIMAGE_THUNK_DATA)((LPBYTE)LoaderParams->ImageBase + pIID->OriginalFirstThunk);
        PIMAGE_THUNK_DATA FirstThunk = (PIMAGE_THUNK_DATA)((LPBYTE)LoaderParams->ImageBase + pIID->FirstThunk);

        HMODULE hModule = LoaderParams->fnLoadLibraryA((LPCSTR)LoaderParams->ImageBase + pIID->Name);

        if (!hModule)
            return FALSE;

        while (OrigFirstThunk->u1.AddressOfData)
        {
            if (OrigFirstThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
            {
                // Import by ordinal
                uintptr_t Function = (uintptr_t)LoaderParams->fnGetProcAddress(hModule,
                    (LPCSTR)(OrigFirstThunk->u1.Ordinal & 0xFFFF));

                if (!Function)
                    return FALSE;

                FirstThunk->u1.Function = Function;
            }
            else
            {
                // Import by name
                PIMAGE_IMPORT_BY_NAME pIBN = (PIMAGE_IMPORT_BY_NAME)((LPBYTE)LoaderParams->ImageBase + OrigFirstThunk->u1.AddressOfData);
                uintptr_t Function = (uintptr_t)LoaderParams->fnGetProcAddress(hModule, (LPCSTR)pIBN->Name);
                if (!Function)
                    return FALSE;

                FirstThunk->u1.Function = Function;
            }
            OrigFirstThunk++;
            FirstThunk++;
        }
        pIID++;
    }

    if (LoaderParams->NtHeaders->OptionalHeader.AddressOfEntryPoint)
    {
        dllmain EntryPoint = (dllmain)((LPBYTE)LoaderParams->ImageBase + LoaderParams->NtHeaders->OptionalHeader.AddressOfEntryPoint);
        return (DWORD)EntryPoint((HMODULE)LoaderParams->ImageBase, DLL_PROCESS_ATTACH, NULL);
    }

    return TRUE;
}

DWORD __stdcall stub(void)
{
    return 0;
}

unsigned inject_ManualMap(DWORD process_id, const char* restrict dll_path)
{
    loaderdata LoaderParams;
    TCHAR abs_dll_path[MAX_PATH];

    GetFullPathName(dll_path, MAX_PATH, abs_dll_path, NULL);

    HANDLE hFile = CreateFileA(abs_dll_path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, 0, NULL);

    DWORD FileSize = GetFileSize(hFile, NULL);
    PVOID FileBuffer = VirtualAlloc(NULL, FileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    ReadFile(hFile, FileBuffer, FileSize, NULL, NULL);

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)FileBuffer + pDosHeader->e_lfanew);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    
    PVOID ExecutableImage = VirtualAllocEx(hProcess, NULL, pNtHeaders->OptionalHeader.SizeOfImage,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // Copy the headers to target process
    WriteProcessMemory(hProcess, ExecutableImage, FileBuffer,
        pNtHeaders->OptionalHeader.SizeOfHeaders, NULL);

    // Target dll_full_path's Section Header & copy sections of the dll to the target
    PIMAGE_SECTION_HEADER pSectHeader = (PIMAGE_SECTION_HEADER)(pNtHeaders + 1);
    for (int i = 0; i < pNtHeaders->FileHeader.NumberOfSections; i++)
    {
        WriteProcessMemory(hProcess, (PVOID)((LPBYTE)ExecutableImage + pSectHeader[i].VirtualAddress),
            (PVOID)((LPBYTE)FileBuffer + pSectHeader[i].PointerToRawData), pSectHeader[i].SizeOfRawData, NULL);
    }

    // Allocating memory for the loader code.
    PVOID LoaderMemory = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);

    LoaderParams.ImageBase = ExecutableImage;
    LoaderParams.NtHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)ExecutableImage + pDosHeader->e_lfanew);
    LoaderParams.BaseReloc = (PIMAGE_BASE_RELOCATION)((LPBYTE)ExecutableImage
        + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
    LoaderParams.ImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)ExecutableImage
        + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    LoaderParams.fnLoadLibraryA = LoadLibraryA;
    LoaderParams.fnGetProcAddress = GetProcAddress;

    // Write the loader information to target process 
    WriteProcessMemory(hProcess, LoaderMemory, &LoaderParams, sizeof(loaderdata), NULL);
    WriteProcessMemory(hProcess, (PVOID)((loaderdata*)LoaderMemory + 1), (LPCVOID)LibraryLoader,
        (uintptr_t)stub - (uintptr_t)LibraryLoader, NULL); // changed from DWORD
    
    
    // Create a remote thread to execute the loader code
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((loaderdata*)LoaderMemory + 1),
        LoaderMemory, 0, NULL);

    // Wait for the loader to finish executing
    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, LoaderMemory, 0, MEM_RELEASE);

    return FALSE;
}
