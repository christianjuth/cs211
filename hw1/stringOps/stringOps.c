#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) 
{
  char *input = argv[1];

  int outputIndex = 0;
  char output[100] = "";
  char vowels[] = "aeiouAEIOU";

  int i;
  for (i = 0; i < strlen(input); i++) {
    char letter = input[i];

    int j;
    for (j = 0; j < strlen(vowels); j++) {
      char vowel = vowels[j];

      if (letter == vowel) {
        output[outputIndex] = letter;
        outputIndex++;
        break;
      }
    }  
  }

  printf("%s\n", output);

  return 0;
}
