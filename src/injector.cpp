#include "definitions.hpp"

#include "loadlibrary.hpp"
#include "manual_map.hpp"
#include "injector.hpp"
#include "logger.hpp"
#include "crt.hpp"

#include <stdio.h>

static inline
int __handle_error(std::int32_t inject_code) {
    switch (inject_code)
    {
        case inject::incorrect_parameters:
        {
            fprintf(stderr, "Error code %d: Incorrect Paramters.\n", inject::incorrect_parameters);
            break;
        }
        case inject::process_not_running:
        {
            fprintf(stderr, "Error code %d: Process is not running.\n", inject::process_not_running);
            break;
        }
        case inject::dll_does_not_exist:
        {
            fprintf(stderr, "Error code %d: DLL does not exist.\n", inject::dll_does_not_exist);
            break;
        }
        case inject::injection_failed:
        {
            fprintf(stderr, "Error code %d: Injection Failed.\n", inject::dll_does_not_exist);
            break;
        }
        default:
        {
            break;
        }
    }

    return 0;
}

std::int32_t inject_payload(const types::parsed_args_t& args) {
    fprintf(stdout, "Searching for %s...\n", args.process_name.c_str());
    fprintf(stdout, "%s Found.\n\n", args.process_name.c_str());
    
    if (args.delay.count()) {
        printf("Delay(ms): %llu\n\n", args.delay.count());
        Sleep((DWORD)(args.delay.count() & 0xFFFFFFFF));
    }

    int32_t status = inject::injection_failed;
    switch (args.operation)
    {
        case inject::operation::load_library_a:
        {
            status = load_library_a(args);
            break;
        }
        case inject::operation::load_library_w:
        {
            status = load_library_w(args);
            break;
        }
        case inject::operation::manual_map:
        {
            status = inject_manual_map(args);
            break;
        }
        case inject::operation::crt:
        {
            status = create_remote_thread(args);
            break;
        }
        default:
        {
            break;
        }
    }

    status && __handle_error(status);
    LOG_MSG(args, (std::string("Injection: ") + (status ? "Failed" : "Successful")).c_str(), 0);

    return status;
}
