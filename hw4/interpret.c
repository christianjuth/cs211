#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Instruction {
  char command[50]; 
  char arg1[50];
  char arg2[50];
  // int arg3;

  struct Instruction* next; 
};

int main(int argc, char* argv[]) 
{
  int registers[4] = {0, 0, 0, 0}; // ax, bx, cx, dx

  char *TEMP_PATH = argv[1];

  if(access(TEMP_PATH, F_OK) == -1) {
    printf("error");
    return 0;
  }

  FILE* file = fopen(TEMP_PATH, "r"); 
  char output[50] = "";

  // Read instructions

  struct Instruction* front = NULL;
  struct Instruction* crnt = NULL;

  int i = 0; 
  while (fgets(output, 50, file)) {
    // add new node to list
    struct Instruction *newNode = (struct Instruction*)malloc(sizeof(struct Instruction));
    if (front == NULL) {
      front = newNode;
      crnt = newNode;
    }

    else {
      newNode->next = NULL;
      crnt->next = newNode;
      crnt = crnt->next;
    }

    char parseString[100] = "";
    int parseStringIndex = 0;

    int section = 0; // 1: command, 2: arg2?, 3: arg3?
    for (int j = 0; j < sizeof(output); j++) {
      if (output[j] == '\t' || output[j] == ' ' || output[j] == '\n') {
        if (parseStringIndex == 0) {
          continue;
        }

        printf("%s", parseString);
        if (section == 0) {
          strcpy(crnt->command, parseString);
        }
        else if (section == 1) {
          strcpy(crnt->arg1, parseString);
        }
        else if (section == 2) {
          strcpy(crnt->arg2, parseString);
        }
        section++;
        memset(parseString, 0, strlen(parseString));
        parseStringIndex = 0;
      }

      else {
        parseString[parseStringIndex] = output[j];
        parseStringIndex++;
      }
    }

    if (parseStringIndex > 0) {
      if (section == 0) {
        strcpy(crnt->command, parseString);
      }
      else if (section == 1) {
        strcpy(crnt->arg1, parseString);
      }
      else if (section == 2) {
        strcpy(crnt->arg2, parseString);
      }
    }

    i++;
  }

  fclose(file); 


  // Execute instructions


  printf("hello world %d", registers[0]);
}