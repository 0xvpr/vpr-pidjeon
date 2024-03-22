#include "CUnit/Basic.h"
#include "CUnit/Console.h"

#include "definitions.h"
#include "logger.h"

#include <tchar.h>
#include <stdio.h>
#include <stdbool.h>

#define LOG_FILE "test.log"

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

void test_log_basic_CreateLogFile(void)
{
    int bytesWritten = 0;
    Injector injector = {
        .output_file = LOG_FILE,
        .logger = log_basic
    };

    FILE* fp = NULL;
    if ((fp = fopen(LOG_FILE, "r")))
    {
        fclose(fp);
        remove(LOG_FILE);
    }

    bytesWritten = injector.logger(
        &injector,
        "Test of log_basic's create log file",
        0
    );

    CU_ASSERT_TRUE(bytesWritten == 101);
}

void test_log_basic_AppendToLogFile(void)
{
    int bytesWritten = 0;
    Injector injector = {
        .output_file = LOG_FILE,
        .logger = log_basic
    };

    bytesWritten = injector.logger(
        &injector,
        "Test of log_basic's append to log file",
        0
    );

    FILE* fp;
    if ((fp = fopen(LOG_FILE, "r")))
    {
        fclose(fp);
        remove(LOG_FILE);
    }

    CU_ASSERT_TRUE(bytesWritten == 63);
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
    if ( NULL == CU_add_test( pSuite, "log_basic() -> Create log file",                     test_log_basic_CreateLogFile)               ||
         NULL == CU_add_test( pSuite, "log_basic() -> Append to log file",                  test_log_basic_AppendToLogFile)
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run tests 
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();      // Run tests automatically

    unsigned fails = CU_get_number_of_failures();

    // CU_console_run_tests(); // Run tests interactively
    CU_cleanup_registry();

    return (fails > 0);
}
