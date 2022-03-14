#include <stdio.h>

#include <windows.h>

#define ever ;;

typedef struct _S
{
  unsigned int value_to_change;
} S;

static const int constant = 42069;
S s = { constant };

int main(void)
{
    fprintf(stdout, "address of value_to_change = 0x%-6p\n", &(s.value_to_change));

    FILE* fp = NULL;
    if ((fp = fopen("temp.txt", "w")))
    {
        fprintf(fp, "0x%-6p", &(s.value_to_change));
        fclose(fp);
    }

    for (ever)
    {
        if (s.value_to_change != constant)
        {
            return 1;
        }

        Sleep(100);
    }

    remove("temp.txt");
    return 0;
}
