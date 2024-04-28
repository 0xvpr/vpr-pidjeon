#include "definitions.hpp"
#include "loadlibrary.hpp"
#include "manualmap.hpp"
#include "injector.hpp"
#include "logger.hpp"
#include "crt.hpp"

#include <stdio.h>

void __handle_error(std::uint32_t inject_code) {
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
}

void inject_payload(types::resource& res, types::injector& inj) {
    fprintf(stdout, "Searching for %s...\n", res.target_process.data());
    fprintf(stdout, "%s Found.\n\n", res.target_process.data());
    
    if (inj.delay_ms.count()) {
        printf("Delay(ms): %llu\n\n", inj.delay_ms.count());
        Sleep(inj.delay_ms.count());
    }

    inj.status = inject::injection_failed;
    switch (inj.operation)
    {
        case inject::operation::load_library_a:
        {
            inj.status = load_library_a(res, inj);
            break;
        }
        case inject::operation::load_library_w:
        {
            inj.status = load_library_w(res, inj);
            break;
        }
        case inject::operation::manual_map:
        {
            inj.status = inject_manual_map(res, inj);
            break;
        }
        case inject::operation::crt:
        {
            inj.status = create_remote_thread(res, inj);
            break;
        }
        default:
        {
            break;
        }
    }

    if (inj.status != 0) {
        __handle_error(inj.status);
    }
    LOG_MSG(inj, (std::string("Injection: %s.\n") + (inj.status ? "Failed" : "Successful")).c_str(), 0);
}
