#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getLineOfFile(char* TEMP_PATH, int nthLine, char* output, int outputSize) {
  FILE* file = fopen(TEMP_PATH, "r"); 

  int i = 0; 
  while (fgets(output, outputSize, file)) { 
    if(i == nthLine ) 
    { 
      break;
    } 
    i++; 
  } 

  fclose(file); 
}

void printIntArr(int* array, int length) {
  int i;
  for(i = 0; i < length; i++) {
    printf("%d\n", array[i]);
  }
}

int main(int argc, char* argv[]) 
{
  // get length of array
  char lengthStr[100] = "";
  getLineOfFile("./hw1/file1.txt", 0, lengthStr, 100);
  int length = atoi(lengthStr);

  // read array
  char dataStr[100] = "";
  getLineOfFile("./hw1/file1.txt", 1, dataStr, 100);
  int array[length];
  int arrayIndex = 0;

  printf("%s\n\n", dataStr);

  int i = 0;
  char crntData[10];
  int crntIndex = 0; 
  while (1) {
    // write to array
    if (dataStr[i] == ' ' || dataStr[i] == '\n') {
      array[arrayIndex] = atoi(crntData);
      arrayIndex++;
      // reset memory
      memset(crntData, 0, sizeof crntData);
      crntIndex = 0;
    }

    // end of line
    if (dataStr[i] == '\n') {
      break;
    }

    crntData[crntIndex] = dataStr[i];
    crntIndex++;
    i++;
  }


  printIntArr(array, length);

  return 0;
}
