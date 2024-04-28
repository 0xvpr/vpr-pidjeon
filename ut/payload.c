#include <windows.h>

#include <stdio.h>
#include <stdint.h>

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    exit(0);
    FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved); // -Wunused-parameters

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(hInstance);
            CreateThread(0, 0, MainThread, hInstance, 0, 0);
            break;
        }
    }

    return TRUE;
}
