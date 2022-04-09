#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  srand(time(0));

  for (int x = 0; x < 10; x++)
  {
    printf("random:%d\n", rand() % 1000);
  }

  int sum = 11111111 * 9;
  printf("%d\n", sum);

  return 0;
}