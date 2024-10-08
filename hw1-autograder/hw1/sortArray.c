#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    printf("%d\t", array[i]);
  }
}

void swap(int* a, int* b) { 
  int temp = *a; 
  *a = *b; 
  *b = temp; 
} 

void sortArray(int arr[], int length, int direction) {
  int i;
  int j;
  int indexOfMin; 
  
  for (i = 0; i < length - 1; i++) { 
    indexOfMin = i; 
    for (j = i + 1; j < length; j++) {
      if (direction ? arr[j] > arr[indexOfMin] : arr[j] < arr[indexOfMin]) {
        indexOfMin = j; 
      }
    }
    swap(&arr[indexOfMin], &arr[i]); 
  } 
}

int main(int argc, char* argv[]) 
{
  char *file = argv[1];

  if(access(file, F_OK) == -1) {
    printf("error");
    return 0;
  }

  // get length of array
  char lengthStr[100] = "";
  getLineOfFile(file, 0, lengthStr, 100);
  int length = atoi(lengthStr);

  // read array
  char dataStr[100] = "";
  getLineOfFile(file, 1, dataStr, 100);
  int array[length];
  int arrayIndex = 0;

  char crntData[10];
  int crntIndex = 0; 
  int i;
  for (i = 0; i < strlen(dataStr); i++) {
    // write to array
    // FIX THIS: can't expect line to end with \n
    if (dataStr[i] == '\t') {
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
  }

  if (i > 0) {
    array[arrayIndex] = atoi(crntData);
    arrayIndex++;
    // reset memory
    memset(crntData, 0, sizeof crntData);
    crntIndex = 0;
  }

  // split into odd and even and sort
  int odd[length];
  int oddIndex = 0;
  int even[length];
  int evenIndex = 0;

  int j;
  for (j = 0; j < length; j++) {
    int item = array[j];
    if (item % 2 == 0) {
      even[evenIndex] = item;
      evenIndex++;
    }

    else {
      odd[oddIndex] = item;
      oddIndex++;
    }
  }

  sortArray(even, evenIndex, 0);
  sortArray(odd, oddIndex, 1);

  // merge odd and even back into array
  int k = 0;
  for (k = 0; k < length; k++) {
    if (k < evenIndex) {
      array[k] = even[k];
    }

    else {
      array[k] = odd[k - evenIndex];
    }
  }

  printIntArr(array, length);

  return 0;
}