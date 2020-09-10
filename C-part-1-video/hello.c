#include <stdio.h>
#include <stdlib.h>

int squu(int num) {
  return num * num;
}

int main(int argc, char* argv[]) 
{
  int count;

  count = atoi(argv[1]);
  for (int i = 0; i < count; i++) {
    printf("Hello %d\n", i);
  }

  int t = 5;
  // !t flips to to 0
  // think of int > 0 as true and int == 0 as false
  printf("%d\n", !t);

  printf("%d\n", squu(5));
}