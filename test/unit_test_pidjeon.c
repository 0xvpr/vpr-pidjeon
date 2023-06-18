#include "CUnit/Basic.h"
#include "CUnit/Console.h"

#include "definitions.h"
#include "injector.h"
#include "parser.h"
#include "logger.h"

#include <tchar.h>
#include <stdio.h>
#include <stdbool.h>

#define TEST_FILE "test_log.log"

static FILE* temp_file = NULL;

/**
 * The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
**/
int init_suite(void)
{
    if (NULL == (temp_file = fopen("cunit.log", "w")))
    {
        return -1;
    }

    return 0;
}

/** 
 * The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
**/
int clean_suite(void)
{
    if (fclose(temp_file))
    {
        remove("cunit.log");
        return -1;
    }

  temp_file = NULL;
  return 0;
}

void testParseCommandLine_UnspecifiedInjector(void)
{
    int argc = 3;
    char* argv[3] = { "injector.exe", "target.exe", "payload.dll" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_LOAD_LIBRARY_A, ParseCommandLine(argc, argv, &resource, &injector));
}

void testParseCommandLine_SpecifyLoadLibraryA(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-i", "LoadLibraryA" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_LOAD_LIBRARY_A, ParseCommandLine(argc, argv, &resource, &injector));
}

void testParseCommandLine_SpecifyLoadLibraryW(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-i", "LoadLibraryW" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_LOAD_LIBRARY_W, ParseCommandLine(argc, argv, &resource, &injector));
}

void testParseCommandLine_SpecifyManualMap(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-i", "ManualMap" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_MANUAL_MAP, ParseCommandLine(argc, argv, &resource, &injector));
}

void testLogEvent_CreateLogFile(void)
{
    int bytesWritten = 0;
    Injector injector = { .output_file = TEST_FILE };

    FILE* fp = NULL;
    if ((fp = fopen(TEST_FILE, "r")))
    {
        fclose(fp);
        remove(TEST_FILE);
    }

    bytesWritten = LogEvent(&injector,
                            "Test of LogEvent's create log file",
                            0);

    CU_ASSERT_TRUE(bytesWritten == 100);
}

void testLogEvent_AppendToLogFile(void)
{
    int bytesWritten = 0;
    Injector injector = { .output_file = TEST_FILE };

    bytesWritten = LogEvent(&injector,
                            "Test of LogEvent's append to log file",
                            0);


    if ((fp = fopen(TEST_FILE, "r")))
    {
        fclose(fp);
        remove(TEST_FILE);
    }

    CU_ASSERT_TRUE(bytesWritten == 62);
}

void testPayloadInjector86_UnspecifiedInjector(void)
{
    LPTSTR injector_args = _tcsdup(TEXT("./bin/vpr-pidjeon-x86.exe dummy-x86.exe ./lib/payload-x86.dll"));
    DWORD exit_code;

    PROCESS_INFORMATION dummy_pi = { 0 };
    STARTUPINFO dummy_si         = { 0 };
    BOOL bDummyProcess           = 0;
    bDummyProcess = CreateProcess( NULL,
                                   "./bin/dummy-x86.exe",
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
        fprintf(stderr, "Dummy process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    Sleep(500);

    HANDLE hChildStdoutRd, hChildStdoutWr;
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, NULL, 0)) {
        printf("Failed to create pipe: %lu\n", GetLastError());
        CU_ASSERT_FALSE(true);
        return;
    }

    PROCESS_INFORMATION injector_pi = { 0 };
    STARTUPINFO injector_si         = { 0 };

    injector_si.cb = sizeof(STARTUPINFOA);
    injector_si.dwFlags = STARTF_USESTDHANDLES;
    injector_si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    injector_si.hStdOutput = hChildStdoutWr; // Redirect stdout to the write end of the pipe
    injector_si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL bInjectorProcess32         = 0;
    bInjectorProcess32 = CreateProcessA( NULL,
                                         injector_args,
                                         NULL,
                                         NULL,
                                         FALSE,
                                         0,
                                         NULL,
                                         NULL,
                                         &injector_si,
                                         &injector_pi);

    if (!bInjectorProcess32)
    {
        fprintf(stderr, "Injector process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    WaitForSingleObject(dummy_pi.hProcess, 500);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);
    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    CU_ASSERT_EQUAL(EXIT_SUCCESS, exit_code);
}

void testPayloadInjector86_SpecifyLoadLibraryA(void)
{
    LPTSTR injector_args = _tcsdup(TEXT("./bin/vpr-pidjeon-x86.exe dummy-x86.exe ./lib/payload-x86.dll -i LoadLibraryA"));
    DWORD exit_code;

    PROCESS_INFORMATION dummy_pi = { 0 };
    STARTUPINFO dummy_si         = { 0 };
    BOOL bDummyProcess           = 0;
    bDummyProcess = CreateProcess( NULL,
                                   "./bin/dummy-x86.exe",
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
        fprintf(stderr, "Dummy process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    Sleep(500);

    HANDLE hChildStdoutRd, hChildStdoutWr;
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, NULL, 0)) {
        printf("Failed to create pipe: %lu\n", GetLastError());
        CU_ASSERT_FALSE(true);
        return;
    }

    PROCESS_INFORMATION injector_pi = { 0 };
    STARTUPINFO injector_si         = { 0 };

    injector_si.cb = sizeof(STARTUPINFOA);
    injector_si.dwFlags = STARTF_USESTDHANDLES;
    injector_si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    injector_si.hStdOutput = hChildStdoutWr; // Redirect stdout to the write end of the pipe
    injector_si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL bInjectorProcess32         = 0;
    bInjectorProcess32 = CreateProcessA( NULL,
                                         injector_args,
                                         NULL,
                                         NULL,
                                         FALSE,
                                         0,
                                         NULL,
                                         NULL,
                                         &injector_si,
                                         &injector_pi);

    if (!bInjectorProcess32)
    {
        fprintf(stderr, "Injector process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    WaitForSingleObject(dummy_pi.hProcess, 500);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);
    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    CU_ASSERT_EQUAL(EXIT_SUCCESS, exit_code);
}

void testPayloadInjector86_SpecifyLoadLibraryW(void)
{
    LPTSTR injector_args = _tcsdup(TEXT("./bin/vpr-pidjeon-x86.exe dummy-x86.exe ./lib/payload-x86.dll -i LoadLibraryW"));
    DWORD exit_code;

    PROCESS_INFORMATION dummy_pi = { 0 };
    STARTUPINFO dummy_si         = { 0 };
    BOOL bDummyProcess           = 0;
    bDummyProcess = CreateProcess( "./bin/dummy-x86.exe",
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
        fprintf(stderr, "Dummy process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    Sleep(500);

    HANDLE hChildStdoutRd, hChildStdoutWr;
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, NULL, 0)) {
        printf("Failed to create pipe: %lu\n", GetLastError());
        CU_ASSERT_FALSE(true);
        return;
    }

    PROCESS_INFORMATION injector_pi = { 0 };
    STARTUPINFO injector_si         = { 0 };

    injector_si.cb = sizeof(STARTUPINFOA);
    injector_si.dwFlags = STARTF_USESTDHANDLES;
    injector_si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    injector_si.hStdOutput = hChildStdoutWr; // Redirect stdout to the write end of the pipe
    injector_si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL bInjectorProcess32         = 0;
    bInjectorProcess32 = CreateProcessA( NULL,
                                         injector_args,
                                         NULL,
                                         NULL,
                                         FALSE,
                                         0,
                                         NULL,
                                         NULL,
                                         &injector_si,
                                         &injector_pi);

    if (!bInjectorProcess32)
    {
        fprintf(stderr, "Injector process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    WaitForSingleObject(dummy_pi.hProcess, 500);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);
    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    CU_ASSERT_EQUAL(EXIT_SUCCESS, exit_code);
}

void testPayloadInjector86_SpecifyManualMap(void)
{
    LPTSTR injector_args = _tcsdup(TEXT("./bin/vpr-pidjeon-x86.exe dummy-x86.exe ./lib/payload-x86.dll -i ManualMap"));
    DWORD exit_code;

    PROCESS_INFORMATION dummy_pi = { 0 };
    STARTUPINFO dummy_si         = { 0 };
    BOOL bDummyProcess           = 0;
    bDummyProcess = CreateProcess( "./bin/dummy-x86.exe",
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
        fprintf(stderr, "Dummy process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    Sleep(500);

    HANDLE hChildStdoutRd, hChildStdoutWr;
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, NULL, 0)) {
        printf("Failed to create pipe: %lu\n", GetLastError());
        CU_ASSERT_FALSE(true);
        return;
    }

    PROCESS_INFORMATION injector_pi = { 0 };
    STARTUPINFO injector_si         = { 0 };

    injector_si.cb = sizeof(STARTUPINFOA);
    injector_si.dwFlags = STARTF_USESTDHANDLES;
    injector_si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    injector_si.hStdOutput = hChildStdoutWr; // Redirect stdout to the write end of the pipe
    injector_si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL bInjectorProcess32         = 0;
    bInjectorProcess32 = CreateProcessA( NULL,
                                         injector_args,
                                         NULL,
                                         NULL,
                                         FALSE,
                                         0,
                                         NULL,
                                         NULL,
                                         &injector_si,
                                         &injector_pi);
    if (!bInjectorProcess32)
    {
        fprintf(stderr, "Injector process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    WaitForSingleObject(dummy_pi.hProcess, 500);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);
    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    CU_ASSERT_EQUAL(EXIT_SUCCESS, exit_code);
}

void testPayloadInjector64_UnspecifiedInjector(void)
{
    LPTSTR injector_args = _tcsdup(TEXT("./bin/vpr-pidjeon-x64.exe dummy-x64.exe ./lib/payload-x64.dll"));
    DWORD exit_code;

    PROCESS_INFORMATION dummy_pi = { 0 };
    STARTUPINFO dummy_si         = { 0 };
    BOOL bDummyProcess           = 0;
    bDummyProcess = CreateProcess( "./bin/dummy-x64.exe",
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
        fprintf(stderr, "Dummy process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    Sleep(500);

    HANDLE hChildStdoutRd, hChildStdoutWr;
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, NULL, 0)) {
        printf("Failed to create pipe: %lu\n", GetLastError());
        CU_ASSERT_FALSE(true);
        return;
    }

    PROCESS_INFORMATION injector_pi = { 0 };
    STARTUPINFO injector_si         = { 0 };

    injector_si.cb = sizeof(STARTUPINFOA);
    injector_si.dwFlags = STARTF_USESTDHANDLES;
    injector_si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    injector_si.hStdOutput = hChildStdoutWr; // Redirect stdout to the write end of the pipe
    injector_si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL bInjectorProcess64         = 0;
    bInjectorProcess64 = CreateProcessA( NULL,
                                         injector_args,
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
        fprintf(stderr, "Injector process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    WaitForSingleObject(dummy_pi.hProcess, 500);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);
    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    CU_ASSERT_EQUAL(EXIT_SUCCESS, exit_code);
}

void testPayloadInjector64_SpecifyLoadLibraryA(void)
{
    LPTSTR injector_args = _tcsdup(TEXT("./bin/vpr-pidjeon-x64.exe dummy-x64.exe ./lib/payload-x64.dll -i LoadLibraryA"));
    DWORD exit_code;

    PROCESS_INFORMATION dummy_pi = { 0 };
    STARTUPINFO dummy_si         = { 0 };
    BOOL bDummyProcess           = 0;
    bDummyProcess = CreateProcess( "./bin/dummy-x64.exe",
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
        fprintf(stderr, "Dummy process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    Sleep(500);

    HANDLE hChildStdoutRd, hChildStdoutWr;
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, NULL, 0)) {
        printf("Failed to create pipe: %lu\n", GetLastError());
        CU_ASSERT_FALSE(true);
        return;
    }

    PROCESS_INFORMATION injector_pi = { 0 };
    STARTUPINFO injector_si         = { 0 };

    injector_si.cb = sizeof(STARTUPINFOA);
    injector_si.dwFlags = STARTF_USESTDHANDLES;
    injector_si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    injector_si.hStdOutput = hChildStdoutWr; // Redirect stdout to the write end of the pipe
    injector_si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL bInjectorProcess64         = 0;
    bInjectorProcess64 = CreateProcessA( NULL,
                                         injector_args,
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
        fprintf(stderr, "Injector process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    WaitForSingleObject(dummy_pi.hProcess, 500);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);
    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    CU_ASSERT_EQUAL(EXIT_SUCCESS, exit_code);
}

void testPayloadInjector64_SpecifyLoadLibraryW(void)
{
    LPTSTR injector_args = _tcsdup(TEXT("./bin/vpr-pidjeon-x64.exe dummy-x64.exe ./lib/payload-x64.dll -i LoadLibraryW"));
    DWORD exit_code;

    PROCESS_INFORMATION dummy_pi = { 0 };
    STARTUPINFO dummy_si         = { 0 };
    BOOL bDummyProcess           = 0;
    bDummyProcess = CreateProcess( "./bin/dummy-x64.exe",
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
        fprintf(stderr, "Dummy process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    Sleep(500);

    HANDLE hChildStdoutRd, hChildStdoutWr;
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, NULL, 0)) {
        printf("Failed to create pipe: %lu\n", GetLastError());
        CU_ASSERT_FALSE(true);
        return;
    }

    PROCESS_INFORMATION injector_pi = { 0 };
    STARTUPINFO injector_si         = { 0 };

    injector_si.cb = sizeof(STARTUPINFOA);
    injector_si.dwFlags = STARTF_USESTDHANDLES;
    injector_si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    injector_si.hStdOutput = hChildStdoutWr; // Redirect stdout to the write end of the pipe
    injector_si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL bInjectorProcess64         = 0;
    bInjectorProcess64 = CreateProcessA( NULL,
                                         injector_args,
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
        fprintf(stderr, "Injector process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    WaitForSingleObject(dummy_pi.hProcess, 500);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);
    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    CU_ASSERT_EQUAL(EXIT_SUCCESS, exit_code);
}

void testPayloadInjector64_SpecifyManualMap(void)
{
    LPTSTR injector_args = _tcsdup(TEXT("./bin/vpr-pidjeon-x64.exe dummy-x64.exe ./lib/payload-x64.dll -i ManualMap"));
    DWORD exit_code;

    PROCESS_INFORMATION dummy_pi = { 0 };
    STARTUPINFO dummy_si         = { 0 };
    BOOL bDummyProcess           = 0;
    bDummyProcess = CreateProcess( "./bin/dummy-x64.exe",
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
        fprintf(stderr, "Dummy process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    Sleep(500);

    HANDLE hChildStdoutRd, hChildStdoutWr;
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, NULL, 0)) {
        printf("Failed to create pipe: %lu\n", GetLastError());
        CU_ASSERT_FALSE(true);
        return;
    }

    PROCESS_INFORMATION injector_pi = { 0 };
    STARTUPINFO injector_si         = { 0 };

    injector_si.cb = sizeof(STARTUPINFOA);
    injector_si.dwFlags = STARTF_USESTDHANDLES;
    injector_si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    injector_si.hStdOutput = hChildStdoutWr; // Redirect stdout to the write end of the pipe
    injector_si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL bInjectorProcess64         = 0;
    bInjectorProcess64 = CreateProcessA( NULL,
                                         injector_args,
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
        fprintf(stderr, "Injector process not created.\n");
        CU_ASSERT_FALSE(true);
        return;
    }

    WaitForSingleObject(dummy_pi.hProcess, 500);
    GetExitCodeProcess(dummy_pi.hProcess, &exit_code);

    CloseHandle(dummy_pi.hProcess);
    CloseHandle(dummy_pi.hThread);
    CloseHandle(injector_pi.hProcess);
    CloseHandle(injector_pi.hThread);

    CU_ASSERT_EQUAL(EXIT_SUCCESS, exit_code);
}

/**
 * The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
**/
int main(void)
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    /* add a suite to the registry */
    pSuite = CU_add_suite("Test Suite", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ( NULL == CU_add_test( pSuite, "ParseCommandLine(Unspecified) -> LoadLibraryA",      testParseCommandLine_UnspecifiedInjector) ||
         NULL == CU_add_test( pSuite, "ParseCommandLine(LoadLibraryA) -> LoadLibraryA",     testParseCommandLine_SpecifyLoadLibraryA) ||
         NULL == CU_add_test( pSuite, "ParseCommandLine(LoadLibraryW) -> LoadLibraryW",     testParseCommandLine_SpecifyLoadLibraryW) ||
         NULL == CU_add_test( pSuite, "ParseCommandLine(ManualMap) -> ManualMap",           testParseCommandLine_SpecifyManualMap) ||
         NULL == CU_add_test( pSuite, "LogEvent() -> Create log file",                      testLogEvent_CreateLogFile) ||
         NULL == CU_add_test( pSuite, "LogEvent() -> Append to log file",                   testLogEvent_AppendToLogFile) ||
         NULL == CU_add_test( pSuite, "vpr-pidjeon-x86 -> LoadLibraryA",                    testPayloadInjector86_UnspecifiedInjector) ||
         NULL == CU_add_test( pSuite, "vpr-pidjeon-x86 -i LoadLibraryA -> LoadLibraryA",    testPayloadInjector86_SpecifyLoadLibraryA) ||
         NULL == CU_add_test( pSuite, "vpr-pidjeon-x86 -i LoadLibraryW -> LoadLibraryW",    testPayloadInjector86_SpecifyLoadLibraryW) ||
         NULL == CU_add_test( pSuite, "vpr-pidjeon-x86 -i ManualMap -> ManualMap",          testPayloadInjector86_SpecifyManualMap) ||
         NULL == CU_add_test( pSuite, "vpr-pidjeon-x64 -> LoadLibraryA",                    testPayloadInjector64_UnspecifiedInjector) ||
         NULL == CU_add_test( pSuite, "vpr-pidjeon-x64 -i LoadLibraryA -> LoadLibraryA",    testPayloadInjector64_SpecifyLoadLibraryA) ||
         NULL == CU_add_test( pSuite, "vpr-pidjeon-x64 -i LoadLibraryW -> LoadLibraryW",    testPayloadInjector64_SpecifyLoadLibraryW) ||
         NULL == CU_add_test( pSuite, "vpr-pidjeon-x64 -i ManualMap -> ManualMap",          testPayloadInjector64_SpecifyManualMap) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run tests 
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();      // Run tests automatically
    // CU_console_run_tests(); // Run tests interactively
    CU_cleanup_registry();

    return CU_get_error();
}
