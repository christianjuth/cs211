#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// struct Arg {
//   char name[20];
//   struct Arg* next;
// }

struct Instruction {
  char args[4][20];
  struct Instruction* next;
};

struct TmpVar {
  int value;
  char name[20];
  struct TmpVar* next;
};

int getTmpVar(struct TmpVar *crnt, char *name) {
  while (crnt != NULL) {
    if (strcmp(crnt->name, name) == 0) {
      return crnt->value;
    }
    crnt = crnt->next;
  }
  return -1;
}

void setTmpVar(struct TmpVar *crnt, char *name, int value) {
  struct TmpVar *prev = NULL;
  while (crnt != NULL) {
    if (strcmp(crnt->name, name) == 0) {
      crnt->value = value;
      return;
    }
    prev = crnt;
    crnt = crnt->next;
  }

  struct TmpVar* new = (struct TmpVar*)malloc(sizeof(struct TmpVar));
  strcpy(new->name, name);
  new->value = value;
  new->next = NULL;
  prev->next = new;
}


void readParamNames(char *input, char output[][20]) {
  char crnt[20] = "";

  int varIndex = 0;
  int crntLength = 0;
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == '\t' || input[i] == ' ' || input[i] == '\n') {
      strcpy(output[varIndex], crnt);
      memset(crnt, 0, strlen(crnt));
      varIndex++;
      crntLength = 0;
      continue;
    }

    crnt[crntLength] = input[i];
    crntLength++;
  }
  
  if (crntLength > 0) {
    strcpy(output[varIndex], crnt);
  }
}

void setInputValues(int *ref, int numOfValues, int val) {
  int crnt = val;
  for (int i = 0; i < numOfValues; i++) {
    int bit = crnt & 1;
    crnt = crnt >> 1;
    ref[numOfValues - 1 - i] = bit;
  }
}

int getInputVarValue(int *refValues, char refNames[][20], int numOfValues, char *name) {
  for (int i = 0; i < numOfValues; i++) {
    if (strcmp(refNames[i], name) == 0) {
      return refValues[i];
    }
  }
  return -1;
}

int setOutputVarValue(int *refValues, char refNames[][20], int numOfValues, char *name, int value) {
  for (int i = 0; i < numOfValues; i++) {
    if (strcmp(refNames[i], name) == 0) {
      refValues[i] = value;
      return 0;
    }
  }
  return -1;
}

int main(int argc, char* argv[])
{
  char *fileName = argv[1];
  FILE* file = fopen(fileName, "r"); 
  char output[50] = "";
  char tmp[50] = "";

  if(access(fileName, F_OK) == -1) {
    printf("error");
    return 0;
  }

  int inputLength = 0;
  int outputLength = 0;

  fgets(output, 50, file);
  sscanf(output, "INPUTVAR %d %[^\n]s", &inputLength, tmp);
  char inputNames[inputLength][20];
  readParamNames(tmp, inputNames);

  fgets(output, 50, file);
  sscanf(output, "OUTPUTVAR %d %[^\n]s", &outputLength, tmp);
  char outputNames[outputLength][20];
  readParamNames(tmp, outputNames);

  struct Instruction* front = NULL;
  struct Instruction* prev = NULL;

  while (fgets(output, 50, file)) {
    struct Instruction* crnt = (struct Instruction*)malloc(sizeof(struct Instruction)); 
    crnt->next = NULL;
    readParamNames(output, crnt->args);

    if (front == NULL) {
      front = crnt;
    } 

    else if (prev == NULL) {
      front->next = crnt;
      prev = crnt;
    }
    
    else {
      prev->next = crnt;
      prev = crnt;
    }
  }

  int max = 1 << inputLength;
  int inputValues[inputLength];
  int outputValues[inputLength];

  struct Instruction* crnt = NULL;
  struct TmpVar* tmpVarFront = (struct TmpVar*)malloc(sizeof(struct TmpVar));
  tmpVarFront->next = NULL;

  for (int i = 0; i < max; i++) {
    crnt = front;
    setInputValues(inputValues, inputLength, i);

    while (crnt != NULL) {
      int computedValue = 0;

      char command[20];
      strcpy(command, crnt->args[0]);

      int arg1 = getInputVarValue(inputValues, inputNames, inputLength, crnt->args[1]);
      if (arg1 == -1) {
        arg1 = getTmpVar(tmpVarFront, crnt->args[1]);
      }

      char outputName[20] = "";

      // HANDLE NOT
      if (strcmp(command, "NOT") == 0) {
        strcpy(outputName, crnt->args[2]);
        computedValue = arg1 != 1;
      } 
      
      else {
        strcpy(outputName, crnt->args[3]);

        int arg2 = getInputVarValue(inputValues, inputNames, inputLength, crnt->args[2]);
        if (arg2 == -1) {
          arg2 = getTmpVar(tmpVarFront, crnt->args[2]);
        }

        if (strcmp(command, "AND") == 0) {
          computedValue = arg1 & arg2;
        }

        else if (strcmp(command, "OR") == 0) {
          computedValue = arg1 | arg2;
        }

        else if (strcmp(command, "NAND") == 0) {
          computedValue = (arg1 & arg2) != 1;
        }

        else if (strcmp(command, "NOR") == 0) {
          computedValue = (arg1 | arg2) != 1;
        }

        else if (strcmp(command, "XOR") == 0) {
          if ((arg1 & arg2) == 1) {
            computedValue = 0;
          } else {
            computedValue = (arg1 | arg2);
          }
        }
      }

      if (setOutputVarValue(outputValues, outputNames, outputLength, outputName, computedValue) == -1) {
        setTmpVar(tmpVarFront, outputName, computedValue);
      }
      crnt = crnt->next;
    }

    // print array
    for (int j = 0; j < inputLength; j++) {
      printf("%d ", inputValues[j]);
    }
    for (int j = 0; j < outputLength; j++) {
      printf("%d ", outputValues[j]);
    }
    printf("\n");
  }


  // Cleanup
  // for (int i = 0; i < inputLength; i++) {
  //   free(inputNames[i]);
  // }
  // for (int i = 0; i < outputLength; i++) {
  //   free(outputNames[i]);
  // }
  while (tmpVarFront != NULL) {
    struct TmpVar* next = tmpVarFront->next;
    free(tmpVarFront);
    tmpVarFront = next;
  }
  while (front != NULL) {
    struct Instruction* next = front->next;
    free(front);
    front = next;
  }
}
