#include "logger.h"
#include <time.h>
/*#include <string.h>*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static bool FileExists(const char* file)
{
    FILE* fp;
    if (!(fp = fopen(file, "rb")))
    {
        return false;
    }
    
    return true;
}

const char* InsertSpacing(int shiftwidth)
{
    if (shiftwidth == 0) { return ""; }

    const int k = 4;
    const int depth = k * shiftwidth;
    char* string = (char *)malloc(sizeof(char) * depth);

    int i = 0;
    for (i = 0; i < depth; i++)
    {
        string[i] = ' ';
    }
    string [i] = '\0';

    return string;
}

int LogEvent(Injector* injector, const char* event, int shiftwidth)
{
    int bytesWritten        = 0;
    unsigned int verbosity  = injector->verbosity;
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
            fprintf(stderr, "Error while opening '%s'.", output_file);
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
        fprintf(stderr, "Error while opening '%s'.", output_file);
        return -1;
    }

    return bytesWritten;
}
