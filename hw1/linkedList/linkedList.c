#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

struct Node {
  int data; 
  struct Node* next; 
};

// Instruction is a queue of instructions
struct Instruction {
  char command; // d, i
  int data; 
  struct Instruction* next; 
};

void buildInstructions(char* TEMP_PATH, struct Instruction* front) {
  FILE* file = fopen(TEMP_PATH, "r"); 
  char output[50] = "";

  int i = 0; 
  while (fgets(output, 50, file)) {
    // add new node to list
    if (i > 0) {
      struct Instruction* newNode = (struct Instruction*)malloc(sizeof(struct Instruction));
      newNode->next = NULL;
      front->next = newNode;
      front = front->next;
    }

    char command = output[0];

    char number[48] = "";
    int j;
    // skip j = 1 since that is the tab character
    for (j = 0; j < sizeof(output) - 2; j++) {
      if (output[j+2] == '\n') {
        break;
      }
      number[j] = output[j+2];
    }

    front->command = command;
    front->data = atoi(number);

    i++;
  }

  fclose(file); 
}

void executeInstruction(struct Instruction* instruction, struct Node* node) {
  struct Node* prevNode = node;
  node = node->next;
  
  while (1) { 
    // insert at end of list
    if (strcmp(&(instruction->command), "i") == 0 && node == NULL) {
      node = (struct Node*)malloc(sizeof(struct Node));
      node->data = instruction->data;
      node->next = NULL;
      prevNode->next = node;
      break;
    }

    // insert between prevNode and node
    if (strcmp(&(instruction->command), "i") == 0 && node->data > instruction->data) {
      struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
      newNode->data = instruction->data;
      newNode->next = node;
      prevNode->next = newNode;
      break;
    }

    if (strcmp(&(instruction->command), "d") == 0 && node != NULL && node->data == instruction->data) {
      prevNode->next = node->next;
      free(node);
      break;
    }

    if (node == NULL) {
      break;
    }
    prevNode = node;
    node = node->next;
  } 
}

void buildList(struct Instruction* instructionFront, struct Node* nodeFront) {
  while (instructionFront != NULL) { 
    executeInstruction(instructionFront, nodeFront);
    instructionFront = instructionFront->next; 
  } 
}

void printList(struct Node* front) {
  if (front == NULL) {
    return;
  }
  printf("%d\n", front->data);
  printList(front->next);
}

int main(int argc, char* argv[]) 
{
  // get instructions from file
  struct Instruction* instructionFront; 
  instructionFront = (struct Instruction*)malloc(sizeof(struct Instruction));
  buildInstructions("./hw1/linkedList/file1.txt", instructionFront);

  // take instructions and use them to build our linked list
  struct Node* nodeFront = NULL;
  nodeFront = (struct Node*)malloc(sizeof(struct Node));
  nodeFront->next = NULL;
  buildList(instructionFront, nodeFront);

  printList(nodeFront->next); 

  return 0; 
}