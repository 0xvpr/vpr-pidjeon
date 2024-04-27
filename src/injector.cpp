#include "definitions.hpp"
#include "loadlibrary.hpp"
#include "manualmap.hpp"
#include "injector.hpp"
#include "parser.hpp"
#include "util.hpp"
#include "mem.hpp"
#include "crt.hpp"

#include <stdio.h>

void __handle_error(unsigned inject_code) {
    switch (inject_code)
    {
        case inject::incorrect_parameters:
            fprintf(stdout, "Error code %d: Incorrect Paramters.\n", inject::incorrect_parameters);
            break;
        case inject::process_not_running:
            fprintf(stdout, "Error code %d: Process is not running.\n", inject::process_not_running);
            break;
        case inject::dll_does_not_exist:
            fprintf(stdout, "Error code %d: DLL does not exist.\n", inject::dll_does_not_exist);
            break;
        case inject::injection_failed:
            fprintf(stdout, "Error code %d: Injection Failed.\n", inject::dll_does_not_exist);
            break;
        default:
            break;
    }
}

void inject_payload(resource& res, injector& inj) {
    fprintf(stdout, "Searching for %s...\n", res.target_process);
    //while (!res.process_id) {
        //res.process_id = get_process_id_by_process_name(res.target_process);
        //Sleep(100);
    //}
    fprintf(stdout, "%s Found.\n\n", res.target_process);
    
    if (inj.delay_ms) {
        printf("Delay(ms): %d\n\n", inj.delay_ms);
        Sleep(inj.delay_ms);
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
    printf("Injection: %s.\n", (inj.status ? "Failed" : "Successful"));
}
