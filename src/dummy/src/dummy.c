#include <stdio.h>

#include <windows.h>

static const int k = 0x42069;
static const char * tmp_filename = "temp.txt";

struct _S
{
    char pad[16];
    unsigned int value_to_change;
} s = { { 0 }, k };

int main(void)
{
    fprintf(stdout, "address of value_to_change = 0x%-6p\n", (void *)&(s.value_to_change));

    FILE* fp = NULL;
    if (!(fp = fopen(tmp_filename, "w")))
    {
        return 1;
    }

    fprintf(fp, "0x%-6p", (void *)&(s.value_to_change));
    fclose(fp);

    for (;;)
    {
        if (s.value_to_change != k)
        {
            remove(tmp_filename);
            return 0;
        }

        Sleep(100);
    }

    remove(tmp_filename);
    return 1;
}
