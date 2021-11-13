#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main(void)
{
    STARTUPINFO dummy_si         = { 0 };
    PROCESS_INFORMATION dummy_pi = { 0 };
    BOOL bDummyProcess           = 0;

    STARTUPINFO injector_si         = { 0 };
    PROCESS_INFORMATION injector_pi = { 0 };
    BOOL bInjectorProcess64           = 0;

    /*RtlSecureZeroMemory(&dummy_si, sizeof(STARTUPINFO));*/
    /*RtlSecureZeroMemory(&dummy_pi, sizeof(STARTUPINFO));*/

    bDummyProcess = CreateProcess( "./dummy.exe",
                                   NULL,
                                   NULL,
                                   NULL,
                                   FALSE,
                                   0,
                                   NULL,
                                   NULL,
                                   &dummy_si,
                                   &dummy_pi);

    if (!bDummyProcess)
    {
        fprintf(stderr, "Failed to start dummy.exe.\n");
        return -1;
    }

    fprintf(stdout, "Process id: %lu\n", dummy_pi.dwProcessId);
    fprintf(stdout, "Thread id: %lu\n", dummy_pi.dwThreadId);

    Sleep(5000);

    LPTSTR szCmdline = _tcsdup(TEXT("./bin/payload_injector_x64.exe dummy.exe payload.dll -i LoadLibraryA"));
    bInjectorProcess64 = CreateProcessA( NULL,
                                         szCmdline,
                                         NULL,
                                         NULL,
                                         FALSE,
                                         0,
                                         NULL,
                                         NULL,
                                         &injector_si,
                                         &injector_pi);

    if (!bInjectorProcess64)
    {
        fprintf(stderr, "Failed to launch injector.\n");
        return -1;
    }


    DWORD exit_code = 0;
    WaitForSingleObject(dummy_pi.hProcess, INFINITE);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);

    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    fprintf(stdout, "Exit code: %lu\n", exit_code);

    return exit_code;
}
