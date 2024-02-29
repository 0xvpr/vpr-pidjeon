/**
 * Creator by:      VPR
 * Created:         September 14, 2021
 *
 * Updated by:      VPR
 * Updated:         June 22nd, 2023
 *
 * Description:     A command line utility capable of injecting arbitrary code into  
 *                  live running processes via a multitude of techniques.
**/

#include "definitions.h"
#include "injector.h"
#include "parser.h"

#include <stdio.h>
#include <errno.h>

int main(int argc, char** argv)
{
    Resource resource = { 0 };
    Injector injector = { .status      = 0,
                          .operation   = 0,
                          .delay_ms    = 0,
                          .remote      = 0,
                          .silent      = 0,
                          .stealth     = 0,
                          .verbosity   = 1,
                          .output_file = "log.txt" };

    if (ParseCommandLine(argc, argv, &resource, &injector) == -1)
    {
        return errno;
    }

    InjectPayload(&resource, &injector);

    return 0;
}
