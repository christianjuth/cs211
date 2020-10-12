#include <stdio.h>
#include <stdlib.h>

void convertToString(int x, char *output) {
  int crnt = x;

  for (int i = 15; i >= 0; i--) {
    if ((crnt & 1) == 1) {
      output[i] = '1';
    } else {
      output[i] = '0';
    }

    crnt = crnt >> 1;
  }
}

void reverseString(char *input, char *output) {
  for (int i = 0; i < 15; i++)  {
    output[i] = input[15 - i];
  }
}

int isEqual(char *input, char *output) {
  for (int i = 0; i < 15; i++)  {
    if (input[i] != output[i]) {
      return 0;
    }
  }
  return 1;
}

int main(int argc, char* argv[])
{
  int x = atoi(argv[1]);

  char output[16] = "";
  convertToString(x, output);

  char reverseOutput[16] = "";
  reverseString(output, reverseOutput);

  if (isEqual(output, reverseOutput)) {
    printf("Is-Palindrome");
  } else {
    printf("Not-Palindrome");
  }
}