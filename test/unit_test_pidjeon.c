#include "CUnit/Console.h"
#include "CUnit/Basic.h"

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
        return -1;
    }

  temp_file = NULL;
  remove("cunit.log");
  return 0;
}

void test_ParseCommandLine_UnspecifiedInjector(void)
{
    int argc = 3;
    char* argv[3] = { "injector.exe", "target.exe", "payload.dll" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_LOAD_LIBRARY_A, ParseCommandLine(argc, argv, &resource, &injector));
}

void test_ParseCommandLine_SpecifyLoadLibraryA(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-i", "LoadLibraryA" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_LOAD_LIBRARY_A, ParseCommandLine(argc, argv, &resource, &injector));
}

void test_ParseCommandLine_SpecifyLoadLibraryW(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-i", "LoadLibraryW" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_LOAD_LIBRARY_W, ParseCommandLine(argc, argv, &resource, &injector));
}

void test_ParseCommandLine_SpecifyManualMap(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-i", "ManualMap" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_MANUAL_MAP, ParseCommandLine(argc, argv, &resource, &injector));
}

void test_ParseCommandLine_CreateRemoteThreadShortOption(void)
{
    int argc = 4;
    char* argv[4] = { "injector.exe", "target.exe", "payload.bin", "-r" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_CRT, ParseCommandLine(argc, argv, &resource, &injector));
}

void test_ParseCommandLine_CreateRemoteThreadLongOption(void)
{
    int argc = 4;
    char* argv[4] = { "injector.exe", "target.exe", "payload.bin", "--crt" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    CU_ASSERT_EQUAL(INJECT_CRT, ParseCommandLine(argc, argv, &resource, &injector));
}

void test_ParseCommandLine_AddDelay1000(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-d", "1000" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    (void)ParseCommandLine(argc, argv, &resource, &injector);
    CU_ASSERT_EQUAL(injector.delay_ms, 1000u);
}

void test_ParseCommandLine_AddDelay100000(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-d", "100000" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    (void)ParseCommandLine(argc, argv, &resource, &injector);
    CU_ASSERT_EQUAL(injector.delay_ms, 100000u);
}

void test_ParseCommandLine_AddDelay100000000(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-d", "100000000" };

    Resource resource = { 0 };
    Injector injector = { .output_file = "log.txt" };

    (void)ParseCommandLine(argc, argv, &resource, &injector);
    CU_ASSERT_EQUAL(injector.delay_ms, 100000000u);
}

void test_LogEvent_CreateLogFile(void)
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

void test_LogEvent_AppendToLogFile(void)
{
    int bytesWritten = 0;
    Injector injector = { .output_file = TEST_FILE };

    bytesWritten = LogEvent(&injector,
                            "Test of LogEvent's append to log file",
                            0);


    FILE* fp;
    if ((fp = fopen(TEST_FILE, "r")))
    {
        fclose(fp);
        remove(TEST_FILE);
    }

    CU_ASSERT_TRUE(bytesWritten == 62);
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
    if ( NULL == CU_add_test( pSuite, "ParseCommandLine: Unspecified",     test_ParseCommandLine_UnspecifiedInjector)           ||
         NULL == CU_add_test( pSuite, "ParseCommandLine: -i LoadLibraryA", test_ParseCommandLine_SpecifyLoadLibraryA)           ||
         NULL == CU_add_test( pSuite, "ParseCommandLine: -i LoadLibraryW", test_ParseCommandLine_SpecifyLoadLibraryW)           ||
         NULL == CU_add_test( pSuite, "ParseCommandLine: -i ManualMap",    test_ParseCommandLine_SpecifyManualMap)              ||
         NULL == CU_add_test( pSuite, "ParseCommandLine: -r",              test_ParseCommandLine_CreateRemoteThreadShortOption) ||
         NULL == CU_add_test( pSuite, "ParseCommandLine: --crt",           test_ParseCommandLine_CreateRemoteThreadLongOption)  ||
         NULL == CU_add_test( pSuite, "ParseCommandLine: -d 1000",         test_ParseCommandLine_AddDelay1000)                  ||
         NULL == CU_add_test( pSuite, "ParseCommandLine: -d 100000",       test_ParseCommandLine_AddDelay100000)                ||
         NULL == CU_add_test( pSuite, "ParseCommandLine: -d 1000000000",   test_ParseCommandLine_AddDelay100000000)             ||
         NULL == CU_add_test( pSuite, "LogEvent: Create log file",         test_LogEvent_CreateLogFile)                         ||
         NULL == CU_add_test( pSuite, "LogEvent: Append to log file",      test_LogEvent_AppendToLogFile)
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run tests */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();      /* Run tests automatically */

    unsigned fails = CU_get_number_of_failures();

    /* CU_console_run_tests(); */ /* Run tests interactively */
    CU_cleanup_registry();

    return (fails > 0);
}
