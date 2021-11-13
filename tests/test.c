#include "CUnit/Basic.h"
#include "injector.h"
#include "parser.h"
#include "data.h"

/* Pointer to the file used by the tests. */
static FILE* temp_file = NULL;

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite(void)
{
    if (NULL == (temp_file = fopen("log.txt", "w+")))
    {
        return -1;
    }

    return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite(void)
{
    if (0 != fclose(temp_file))
    {
        return -1;
    }

  temp_file = NULL;
  return 0;
}

void testParseCommandLine_UnspecifiedInjector(void)
{
    int argc = 3;
    char* argv[3] = { "injector.exe", "target.exe", "payload.dll" };
    Injector injector = { .output_file = "log.txt" };
    InjectData data = { 0 };

    CU_ASSERT_EQUAL(INJECT_LOAD_LIBRARY_A, ParseCommandLine(argc, argv, &injector, &data));
}

void testParseCommandLine_SpecifyLoadLibraryA(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-i", "LoadLibraryA" };
    Injector injector = { .output_file = "log.txt" };
    InjectData data = { 0 };

    CU_ASSERT_EQUAL(INJECT_LOAD_LIBRARY_A, ParseCommandLine(argc, argv, &injector, &data));
}

void testParseCommandLine_SpecifyManualMap(void)
{
    int argc = 5;
    char* argv[5] = { "injector.exe", "target.exe", "payload.dll", "-i", "ManualMap" };
    Injector injector = { .output_file = "log.txt" };
    InjectData data = { 0 };

    CU_ASSERT_EQUAL(INJECT_MANUAL_MAP, ParseCommandLine(argc, argv, &injector, &data));
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
    if ( NULL == CU_add_test(pSuite, "test of Unspecified Injector -> LoadLibraryA", testParseCommandLine_UnspecifiedInjector) ||
         NULL == CU_add_test(pSuite, "test of Specified Injector -> LoadLibraryA", testParseCommandLine_SpecifyLoadLibraryA) ||
         NULL == CU_add_test(pSuite, "test of Specified Injector -> ManualMap", testParseCommandLine_SpecifyManualMap) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
