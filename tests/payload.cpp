#include <windows.h>
#include <stdio.h>

unsigned int address = 0x4b0010;

DWORD WINAPI MainThread(LPVOID lpReserved)
{
  *(unsigned int *)address = 1337;

  FreeLibraryAndExitThread((HMODULE)lpReserved, 0);

  return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  switch (dwReason)
  {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hInstance);
      CreateThread(0, 0, MainThread, hInstance, 0, 0);
      break;
  }

  return TRUE;
}
