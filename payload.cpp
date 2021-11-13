#include <windows.h>
#include <stdio.h>

DWORD WINAPI MainThread(LPVOID lpReserved)
{
  FILE* fp;
  AllocConsole();
  freopen_s(&fp, "CONOUT$", "w", stdout);

  while (!GetAsyncKeyState(VK_SPACE))
  {
    //
  }

  fclose(fp);
  FreeConsole();
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
