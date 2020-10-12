#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int comp(int x, int n) { 
  return x ^ (1 << n);
} 

int set(int x, int n, int v) {
  int second = (1 << n);

  if (v == 0) {
    return x & (~second);
  } 
  
  else {
    return x | second;
  }
}

int get(int x, int n) {
  return (x >> n) & 1;
}

int main(int argc, char* argv[])
{

  char *fileName = argv[1];
  FILE* file = fopen(fileName, "r"); 
  char output[50] = "";

  if(access(fileName, F_OK) == -1) {
    printf("error");
    return 0;
  }

  fgets(output, 50, file);
  int inputNumber = atoi(output);

  int i = 1; 
  while (fgets(output, 50, file)) {
    char crnt[100] = "";
    int crntIndex = 0;
    int crntCol = 0;

    char command[100] = "";
    int firstArg;
    int secondArg;

    for (int k = 0; k < strlen(output); k++) {
      if (output[k] == '\n') {
        continue;
      }

      if (output[k] == '\t' || output[k] == ' ') {
        if (crntIndex > 0) {
          if (crntCol == 0) {
            strcpy(command, crnt);
          }

          if (crntCol == 1) {
            firstArg = atoi(crnt);
          }
          
          // reset
          memset(crnt, 0, strlen(crnt));
          crntIndex = 0;
          crntCol++;
        }

        // skip over tab
        continue;
      }

      crnt[crntIndex] = output[k];
      crntIndex++;
    }
    secondArg = atoi(crnt);

    int output;
    if (strcmp(command, "set") == 0) {
      output = set(inputNumber, firstArg, secondArg);
      inputNumber = output;
    }

    if (strcmp(command, "comp") == 0) {
      output = comp(inputNumber, firstArg);
      inputNumber = output;
    }

    if (strcmp(command, "get") == 0) {
      output = get(inputNumber, firstArg);
    }

    printf("%d\n", output);

    i++;
  }
  fclose(file); 
}