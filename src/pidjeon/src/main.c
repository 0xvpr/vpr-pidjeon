/**
 * Author:   VPR
 * Created:  September 14, 2021
 * Modified: November 14, 2021
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
