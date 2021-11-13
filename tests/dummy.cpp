#include <iostream>
#include <fstream>
#include <windows.h>

static const int constant = 42069;

struct s
{
  unsigned int value_to_change = constant;
} s;

int main()
{
  std::cout << "address of value_to_change = " << std::hex << &(s.value_to_change) << "\n";

  FILE* fp = NULL;
  if ((fp = fopen("temp.txt", "w")))
  {
    fprintf(fp, "0x%-6p", &(s.value_to_change));
    fclose(fp);
  }

  while (true)
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
