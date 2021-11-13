/**
 * Author:   VPR
 * Created:  September 14, 2021
 * Modified: November 12, 2021
**/

#include "definitions.h"
#include "injector.h"
#include "parser.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    InjectData data   = { 0 };
    Injector injector = { .status      = 0,
                          .operation   = 0,
                          .delay_ms    = 0,
                          .remote      = 0,
                          .silent      = 0,
                          .stealth     = 0,
                          .verbosity   = 1,
                          .output_file = "log.txt" };

    if (ParseCommandLine(argc, argv, &injector, &data) == -1)
    {
        return -1;
    }

    InjectPayload(&data, &injector);

    return 0;
}
