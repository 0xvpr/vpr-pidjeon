#include "logger.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int log_basic(const Injector* injector, const char* restrict event, uint32_t shiftwidth)
{
    (void)injector;
    int bytes_written = 0;

    time_t rawtime = 0;
    struct tm* info = NULL;

    time(&rawtime);
    info = localtime(&rawtime);

    char prefix[64] = { 0 };
    strftime(prefix, 80, "[%Y-%m-%d %H:%M:%S]:", info);
    
    while (shiftwidth--) bytes_written += fputc(' ', stdout);
    bytes_written += fprintf(stdout, "%s %s.\n", prefix, event);

    return bytes_written;
}

int log_advanced(const Injector* injector, const char* restrict event, unsigned shiftwidth)
{
    int bytesWritten        = 0;
    int verbosity           = injector->verbosity;
    const char* output_file = injector->output_file;

    FILE* fp = NULL;
    if (!file_exists(output_file))
    {
        if ((fp = fopen(output_file, "wb")))
        {
            time_t rawtime = 0;
            struct tm* info = NULL;

            time(&rawtime);
            info = localtime(&rawtime);

            char prefix[64] = { 0 };
            strftime(prefix, 80, "[%Y-%m-%d %H:%M:%S]:", info);
            
            while (shiftwidth--) fputc(' ', fp);
            bytesWritten += fprintf(fp, "%s Log file created.\n", prefix);
            fclose(fp);
        }
        else
        {
            if (verbosity > 2)
            {
                fprintf(stderr, "Error while opening '%s'.", output_file);
            }

            return -1;
        }
    }

    if ((fp = fopen(output_file, "ab")))
    {
        time_t rawtime = 0;
        struct tm* info = NULL;

        time(&rawtime);
        info = localtime(&rawtime);

        char prefix[64] = { 0 };
        strftime(prefix, 80, "[%Y-%m-%d %H:%M:%S]:", info);
        
        while (shiftwidth--) fputc(' ', fp);
        bytesWritten += fprintf(fp, "%s %s.\n", prefix, event);
        fclose(fp);
    }
    else
    {
        if (verbosity > 1)
        {
            fprintf(stderr, "Error while opening '%s'.", output_file);
        }
        return -1;
    }

    return bytesWritten;
}
