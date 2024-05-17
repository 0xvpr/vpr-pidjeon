/**
 * Created by:  VPR
 * Created:     September 14th, 2021
 *
 * Updated by:  VPR
 * Updated:     May 17th, 2024
 *
 * Description: Payload injection tool.
**/

#include   "parser.h"
#include   "logger.h"
#include   "util.h"

#include   <stdio.h>

static inline
int __handle_status(enum status_t inject_code)
{
    switch (inject_code)
    {
        case status_incorrect_parameters:
        {
            fprintf(stderr, "Error code %d: Incorrect Paramters.\n", status_incorrect_parameters);
            break;
        }
        case status_process_not_running:
        {
            fprintf(stderr, "Error code %d: Process is not running.\n", status_process_not_running);
            break;
        }
        case status_dll_does_not_exist:
        {
            fprintf(stderr, "Error code %d: DLL does not exist.\n", status_dll_does_not_exist);
            break;
        }
        case status_injection_failed:
        {
            fprintf(stderr, "Error code %d: Injection Failed.\n", status_dll_does_not_exist);
            break;
        }
        default:
        {
            break;
        }
    }

    return 0;
}

static inline
int __handle_error(enum errcode_t err)
{
    switch (err)
    {
        case err_invalid_arguments:
        {
            fprintf(stderr, "Error code %d: Incorrect Paramters.\n", err_invalid_arguments);
            break;
        }
        case err_missing_arguments:
        {
            fprintf(stderr, "Error code %d: Process is not running.\n", err_missing_arguments);
            break;
        }
        case err_unsupported_arguments:
        {
            fprintf(stderr, "Error code %d: DLL does not exist.\n", err_unsupported_arguments);
            break;
        }
        case err_parse_error:
        {
            fprintf(stderr, "Error code %d: Injection Failed.\n", err_parse_error);
            break;
        }
        default:
        {
            break;
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    struct parsed_args_t args = { 0 };
    init_parsed_args(&args, *argv);

    enum errcode_t err = parse_arguments(argc, argv, &args);
    err && __handle_error(err);

    err = validate_arguments(&args); 
    err && __handle_error(err);

    if (args.delay)
    {
        fprintf(stdout, "Delay(ms): %llu\n\n", args.delay);
        Sleep((DWORD)args.delay);
    }

    enum status_t status = args.operation(&args);
    status != status_success && __handle_status(status);

    char buffer[64] = { 0 };
    snprintf(buffer, sizeof(buffer)-1, "Injection: %s", status ? "failed" : "successful");
    LOG_MSG((&args), buffer, 0);


    return 0;
}
