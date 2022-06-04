#include "logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static int FileExists(const char* restrict file)
{
    FILE* fp;
    if (!(fp = fopen(file, "rb")))
    {
        return 0;
    }
    
    return 1;
}

const char* InsertSpacing(unsigned shiftwidth)
{
    if (shiftwidth == 0)
    {
        return NULL;
    }

    const unsigned k = 4;
    const unsigned depth = k * shiftwidth;
    char* string = (char *)malloc(sizeof(char) * depth);

    unsigned i = 0;
    for (i = 0; i < depth; i++)
    {
        string[i] = ' ';
    }
    string [i] = '\0';

    return string;
}

int LogEvent(Injector* injector, const char* restrict event, unsigned shiftwidth)
{
    int bytesWritten        = 0;
    int verbosity           = injector->verbosity;
    const char* output_file = injector->output_file;

    FILE* fp = NULL;
    if (!FileExists(output_file))
    {
        if ((fp = fopen(output_file, "a")))
        {
            time_t rawtime = 0;
            struct tm* info = NULL;

            time(&rawtime);
            info = localtime(&rawtime);

            char prefix[64] = { 0 };
            strftime(prefix, 80, "[%Y-%m-%d %H:%M:%S]:", info);
            
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

    if ((fp = fopen(output_file, "a")))
    {
        time_t rawtime = 0;
        struct tm* info = NULL;

        time(&rawtime);
        info = localtime(&rawtime);

        char prefix[64] = { 0 };
        strftime(prefix, 80, "[%Y-%m-%d %H:%M:%S]:", info);
        
        bytesWritten += fprintf(fp, "%s%s %s.\n", InsertSpacing(shiftwidth), prefix, event);
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
