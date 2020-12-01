#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Instruction {
  char command[50]; 
  char arg1[50];
  char arg2[50];
  char arg3[50];

  struct Instruction* next; 
};

void writeToRegister(int registers[4], char reg, int val) {
  if (reg == 'a') {
    registers[0] = val;
  } else if (reg == 'b') {
    registers[1] = val;
  } else if (reg == 'c') {
    registers[2] = val;
  } else if (reg == 'd') {
    registers[3] = val;
  }
}

int readRegister(int registers[4], char reg) {
  if (reg == 'a') {
    return registers[0];
  } else if (reg == 'b') {
    return registers[1];
  } else if (reg == 'c') {
    return registers[2];
  } else if (reg == 'd') {
    return registers[3];
  }
  return -1;
}

struct Instruction* nthInstruction(struct Instruction* front, int nth) {
  struct Instruction* crnt = front;
  int i = 0;
  while (i < nth && crnt != NULL) {
    crnt = crnt->next;
    i++;
  }
  return crnt;
}

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

        if (section == 0) {
          strcpy(crnt->command, parseString);
        }
        else if (section == 1) {
          strcpy(crnt->arg1, parseString);
        }
        else if (section == 2) {
          strcpy(crnt->arg2, parseString);
        }
        else if (section == 3) {
          strcpy(crnt->arg3, parseString);
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
      else if (section == 3) {
        strcpy(crnt->arg3, parseString);
      }
    }

    i++;
  }

  fclose(file); 


  // Execute instructions
  crnt = front;
  while (crnt != NULL) {
    // Basic commands
    if (strcmp(crnt->command, "mov") == 0) {
      int val;
      if (crnt->arg1[1] == 'x') {
        val = readRegister(registers, crnt->arg1[0]);
      } else {
        val = atoi(crnt->arg1);
      }
      writeToRegister(registers, crnt->arg2[0], val);
    } 

    if (strcmp(crnt->command, "read") == 0) {
      int val;
      scanf("%d", &val);
      writeToRegister(registers, crnt->arg1[0], val);
    } 

    if (strcmp(crnt->command, "add") == 0) {
      int first;
      if (crnt->arg1[1] == 'x') {
        first = readRegister(registers, crnt->arg1[0]);
      } else {
        first = atoi(crnt->arg1);
      }
      int second = readRegister(registers, crnt->arg2[0]);
      writeToRegister(registers, crnt->arg2[0], first + second);
    }

    if (strcmp(crnt->command, "sub") == 0) {
      int first;
      if (crnt->arg1[1] == 'x') {
        first = readRegister(registers, crnt->arg1[0]);
      } else {
        first = atoi(crnt->arg1);
      }
      int second = readRegister(registers, crnt->arg2[0]);
      writeToRegister(registers, crnt->arg2[0], second - first);
    }

    if (strcmp(crnt->command, "mul") == 0) {
      int first;
      if (crnt->arg1[1] == 'x') {
        first = readRegister(registers, crnt->arg1[0]);
      } else {
        first = atoi(crnt->arg1);
      }
      int second = readRegister(registers, crnt->arg2[0]);
      writeToRegister(registers, crnt->arg2[0], second * first);
    }

    if (strcmp(crnt->command, "div") == 0) {
      int first;
      if (crnt->arg1[1] == 'x') {
        first = readRegister(registers, crnt->arg1[0]);
      } else {
        first = atoi(crnt->arg1);
      }
      int second = readRegister(registers, crnt->arg2[0]);
      writeToRegister(registers, crnt->arg2[0], first / second);
    }

    if (strcmp(crnt->command, "print") == 0) {
      int val;
      if (crnt->arg1[1] == 'x') {
        val = readRegister(registers, crnt->arg1[0]);
      } else {
        val = atoi(crnt->arg1);
      }
      printf("%d", val);
    }

    // Jump commands
    if (strcmp(crnt->command, "jmp") == 0) {
      crnt = nthInstruction(front, atoi(crnt->arg1) );
      continue;
    }

    if (strcmp(crnt->command, "je") == 0) {
      int first;
      if (crnt->arg2[1] == 'x') {
        first = readRegister(registers, crnt->arg2[0]);
      } else {
        first = atoi(crnt->arg2);
      }
      int second;
      if (crnt->arg3[1] == 'x') {
        second = readRegister(registers, crnt->arg3[0]);
      } else {
        second = atoi(crnt->arg3);
      }
      if (first == second) {
        crnt = nthInstruction(front, atoi(crnt->arg1) );
        continue;
      }
    }

    if (strcmp(crnt->command, "jne") == 0) {
      int first;
      if (crnt->arg2[1] == 'x') {
        first = readRegister(registers, crnt->arg2[0]);
      } else {
        first = atoi(crnt->arg2);
      }
      int second;
      if (crnt->arg3[1] == 'x') {
        second = readRegister(registers, crnt->arg3[0]);
      } else {
        second = atoi(crnt->arg3);
      }
      if (first != second) {
        crnt = nthInstruction(front, atoi(crnt->arg1) );
        continue;
      }
    }

    if (strcmp(crnt->command, "jg") == 0) {
      int first;
      if (crnt->arg2[1] == 'x') {
        first = readRegister(registers, crnt->arg2[0]);
      } else {
        first = atoi(crnt->arg2);
      }
      int second;
      if (crnt->arg3[1] == 'x') {
        second = readRegister(registers, crnt->arg3[0]);
      } else {
        second = atoi(crnt->arg3);
      }
      if (first > second) {
        crnt = nthInstruction(front, atoi(crnt->arg1) );
        continue;
      }
    }

    if (strcmp(crnt->command, "jge") == 0) {
      int first;
      if (crnt->arg2[1] == 'x') {
        first = readRegister(registers, crnt->arg2[0]);
      } else {
        first = atoi(crnt->arg2);
      }
      int second;
      if (crnt->arg3[1] == 'x') {
        second = readRegister(registers, crnt->arg3[0]);
      } else {
        second = atoi(crnt->arg3);
      }
      if (first >= second) {
        crnt = nthInstruction(front, atoi(crnt->arg1) );
        continue;
      }
    }

    if (strcmp(crnt->command, "jl") == 0) {
      int first;
      if (crnt->arg2[1] == 'x') {
        first = readRegister(registers, crnt->arg2[0]);
      } else {
        first = atoi(crnt->arg2);
      }
      int second;
      if (crnt->arg3[1] == 'x') {
        second = readRegister(registers, crnt->arg3[0]);
      } else {
        second = atoi(crnt->arg3);
      }
      if (first < second) {
        crnt = nthInstruction(front, atoi(crnt->arg1) );
        continue;
      }
    }

    if (strcmp(crnt->command, "jle") == 0) {
      int first;
      if (crnt->arg2[1] == 'x') {
        first = readRegister(registers, crnt->arg2[0]);
      } else {
        first = atoi(crnt->arg2);
      }
      int second;
      if (crnt->arg3[1] == 'x') {
        second = readRegister(registers, crnt->arg3[0]);
      } else {
        second = atoi(crnt->arg3);
      }
      if (first <= second) {
        crnt = nthInstruction(front, atoi(crnt->arg1) );
        continue;
      }
    }

    crnt = crnt->next;
  }
}