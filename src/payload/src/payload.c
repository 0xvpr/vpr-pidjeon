#include <windows.h>

#include <stdio.h>

uintptr_t address = 0;

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    FILE* fp = NULL;
    if ((fp = fopen("temp.txt", "r")))
    {
#ifdef _WIN64
        fscanf(fp, "%llx", &address);
#else
        fscanf(fp, "%x", &address);
#endif
        *(uintptr_t *)address = 1337;
        fclose(fp);
    }

  FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
  return TRUE;
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
