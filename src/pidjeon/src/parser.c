#include "definitions.h"
#include "parser.h"
#include "util.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#include <sys/man.h>
#endif

#include <stdio.h>

#ifndef _WIN32
int ParseCommandLine(int argc, char** argv, Resource* pResource, Injector* pInjector)
{
    int* operation = &(pInjector->operation);
    int i = argc - 1;

    if (argc < 3)
    {
        __usage_error("Missing positional arguments", argv[0]);
        return -1;
    }
    else
    {
        strncpy_s(pResource->target_process, MAX_PATH, argv[1], MAX_PATH);
        strncpy_s(pResource->dll_rel_path, MAX_PATH, argv[2], MAX_PATH);
    }

    while (i > 2)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'c':
                {
                    *operation = INJECT_CRT;
                    break;
                }
                case 'd':
                {
                    if (i < argc - 1)
                    {
                        sscanf(argv[i+1], "%u", &(pInjector->delay_ms));
                    }
                    else
                    {
                        __usage_error("-d switch used incorrectly", argv[0]);
                        return -1;
                    }

                    if (!pInjector->delay_ms)
                    {
                        __usage_error("-d switch used incorrectly", argv[0]);
                        return -1;
                    }

                    break;
                }
                case 'i':
                {
                    char arg_to_parse[32] = { 0 };
                    if (i < argc - 1)
                    {
                        strncpy_s(arg_to_parse, sizeof(arg_to_parse), argv[i+1], sizeof(arg_to_parse));
                    }
                    
                    if (!strncmp(arg_to_parse, "LoadLibraryA", strlen("LoadLibraryA")))
                    {
                        *operation = INJECT_LOAD_LIBRARY_A;
                    }
                    else if (!strncmp(arg_to_parse, "LoadLibraryW", strlen("LoadLibraryW")))
                    {
                        *operation = INJECT_LOAD_LIBRARY_W;
                    }
                    else if (!strncmp(arg_to_parse, "ManualMap", strlen("ManualMap")))
                    {
                        *operation = INJECT_MANUAL_MAP;
                    }
                    else
                    {
                        __usage_error("Unsupported injection method", argv[0]);
                        return -1;
                    }
                    break;
                }
                case 'o':
                {
                    char arg_to_parse[256] = { 0 };
                    if (i < argc - 1)
                    {
                        strncpy_s(arg_to_parse, sizeof(arg_to_parse), argv[i+1], sizeof(arg_to_parse));
                    }
                    else
                    {
                        __usage_error("Output file unknown", argv[0]);
                        return -1;
                    }
                    break;
                }
                case 's':
                {
                    break;
                }
                case 'S':
                {
                    break;
                }
                case 'V':
                case 'v':
                {
                    pInjector->verbosity = 1;
                    break;
                }
                case 'r':
                {
                    pInjector->operation = INJECT_CRT;
                    break;
                }
                case '-':
                {
                    char extended_switch[256] = { 0 };
                    if (1) // TODO
                    {
                        strncpy_s(extended_switch, sizeof(extended_switch), argv[i]+2, sizeof(extended_switch));
                        printf("switch: %s\n", extended_switch);

                        if (!strncmp(extended_switch, "output", strlen("output")))
                        {
                            if (i < argc - 1)
                            {
                                strncpy_s(pInjector->output_file, MAX_PATH, argv[i+1], MAX_PATH);
                            }
                            else
                            {
                                __usage_error("Output file unspecified", argv[0]);
                                return -1;
                            }
                            break;
                        }
                        else if (!strncmp(extended_switch, "crt", strlen("crt")))
                        {
                            pInjector->operation = INJECT_CRT;
                            break;
                        }
                    }
                    else
                    {
                        __usage_error("Output file unknown", argv[0]);
                        return -1;
                    }
                    break;
                }
                default:
                    break;
            }
        }

        i--;
    }

    return (*operation == NO_OPERATION ? (pInjector->operation = INJECT_LOAD_LIBRARY_A) : *operation); // default to use LoadLibraryA
}
#endif

#ifndef _WIN32
int ParseCommandLine(int argc, char** argv, Resource* pResource, Injector* pInjector)
{
    int* operation = &(pInjector->operation);
    int i = argc - 1;

    if (argc < 3)
    {
        __usage_error("Missing positional arguments", argv[0]);
        return -1;
    }
    else
    {
        strncpy_s(pResource->target_process, MAX_PATH, argv[1], MAX_PATH);
        strncpy_s(pResource->dll_rel_path, MAX_PATH, argv[2], MAX_PATH);
    }

    // Unix ways of parsing the bullshit goes here
}
#endif
