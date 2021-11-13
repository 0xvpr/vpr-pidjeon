#include <windows.h>
#include <stdio.h>

unsigned int address = 0;

DWORD WINAPI MainThread(LPVOID lpReserved)
{
  FILE* fp = NULL;
  if ((fp = fopen("temp.txt", "r")))
  {
    fscanf(fp, "%x", &address);
    *(unsigned int *)address = 1337;
    fclose(fp);
  }

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
