#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Node {
  int data; 
  struct Node* left; 
  struct Node* right; 
};

void insert(struct Node* root, struct Node* node) {
  struct Node* prev = NULL;
  struct Node* crnt = root;

  while (crnt != NULL) {
    if (node->data < crnt->data) {
      prev = crnt;
      crnt = crnt->left;
    } 
    
    else if (node->data > crnt->data) {
      prev = crnt;
      crnt = crnt->right;
    }
    
    else {
      free(node);
      return;
    }
  }

  if (node->data < prev->data) {
    prev->left = node;
  }
  
  else if (node->data > prev->data) {
    prev->right = node;
  }
}

void traverse(struct Node* root) {
  if (root == NULL) {
    return;
  }

  traverse(root->left);
  printf("%d\t", root->data);
  traverse(root->right);

  free(root);
}

int main(int argc, char* argv[])
{
  struct Node* root = NULL;

  // read file
  char *fileName = argv[1];
  FILE* file = fopen(fileName, "r"); 
  char output[50] = "";

  if(access(fileName, F_OK) == -1) {
    printf("error");
    return 0;
  }

  int i = 0; 
  while (fgets(output, 50, file)) {
    // char command = output[0];

    char number[48] = "";
    int j;
    // skip j = 1 since that is the tab character
    for (j = 0; j < sizeof(output) - 2; j++) {
      if (output[j+2] == '\n') {
        break;
      }
      number[j] = output[j+2];
    }

    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = atoi(number);
    node->right = NULL;
    node->left = NULL;

    if (root == NULL) {
      root = node;  
    } else {
      insert(root, node);
    }

    i++;
  }

  fclose(file); 

  traverse(root);
}
