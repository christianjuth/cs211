#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Node {
  int data;
  struct Node* next;
};

int main(int argc, char* argv[])
{
  int size = 10;
  struct Node* hashtable[size];

  for (int k = 0; k < size; k++) {
    hashtable[k] = NULL;
  }

  int collisions = 0;
  int successfulSearches = 0;

  // read file
  char *fileName = argv[1];
  FILE* file = fopen(fileName, "r"); 
  char output[50] = "";

  if(access(fileName, F_OK) == -1) {
    printf("error");
    return 0;
  }

  while (fgets(output, 50, file)) {
    char command = output[0];

    char stringNumber[48] = "";
    int j;
    // skip j = 1 since that is the tab character
    for (j = 0; j < sizeof(output) - 2; j++) {
      if (output[j+2] == '\n') {
        break;
      }
      stringNumber[j] = output[j+2];
    }

    int number = atoi(stringNumber);
    int key = number % size;

    if (command == 'i') {
      struct Node* node = (struct Node*)malloc(sizeof(struct Node));
      node->data = number;
      node->next = NULL;
      
      if (hashtable[key] == NULL) {
        hashtable[key] = node;
      }

      else {
        collisions++;
        struct Node* prev = NULL;
        struct Node* crnt = hashtable[key];
        while (crnt != NULL) {
          prev = crnt;
          crnt = crnt->next;
        }
        
        prev->next = node;
      }
    }

    if (command == 's') {
      struct Node* node = hashtable[key];

      while (node != NULL) {
        if (node->data == number) {
          successfulSearches++;
          break;
        }
        node = node->next;
      }
    }
  }

  fclose(file); 

  printf("%d\n%d", collisions, successfulSearches);

  for (int l = 0; l < size; l++) {
    struct Node* prev = NULL;
    struct Node* node = hashtable[l];

    while (node != NULL) {
      if (prev != NULL) {
        free(prev);
      }
      prev = node;
      node = node->next;
    }

    if (prev != NULL) {
      free(prev);
    }
  }
}
