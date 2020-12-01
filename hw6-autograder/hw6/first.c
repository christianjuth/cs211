#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

struct Node {
  int valid;
  int tag;
};

void hextobin(char hex[]) {  
  int i=0;   
  while(hex[i])  {  
    switch(hex[i]) {  
      case '0':  
        printf("0000");  
        break;  
      case '1':  
        printf("0001");  
        break;  
      case '2':  
        printf("0010");  
        break;  
      case '3':  
        printf("0011");  
        break;  
      case '4':  
        printf("0100");  
        break;  
      case '5':  
        printf("0101");  
        break;  
      case '6':  
        printf("0110");  
        break;  
      case '7':  
        printf("0111");  
        break;  
      case '8':  
        printf("1000");  
        break;  
      case '9':  
        printf("1001");  
        break;  
      case 'a':  
        printf("1010");  
        break;  
      case 'b':  
        printf("1011");  
        break;  
      case 'c':  
        printf("1100");  
        break;  
      case 'd':  
        printf("1101");  
        break;  
      case 'e':  
        printf("1110");  
        break;  
      case 'f':  
        printf("1111");  
        break;  
    }  
    i++;  
  }
}  

int cmpprefix(const char *pre, const char *str) {
  return strncmp(pre, str, strlen(pre));
}

int main(int argc, char* argv[])
{

  // is the total size of the cache in bytes. 
  // This number should be a power of 2
  int cacheSize = atoi(argv[1]);
  int assoc;
  int setNum;

  int blockSize = atoi(argv[4]);

  // | Tag | Set | Offset |
  int offsetNum = log2(blockSize);

  //  simulate a direct mapped cache
  if (strcmp(argv[2], "direct") == 0) {
    assoc = 1;
    setNum = log2(cacheSize / blockSize);
  } 

  // simulate a fully associative cache
  else if (strcmp(argv[2], "assoc") == 0) {
    assoc = cacheSize / blockSize;
    setNum = log2(1);
  } 
  
  // simulate an n-way associative cache. n will be a power of 2
  else if(cmpprefix("assoc:", argv[2]) == 0) {
    sscanf(argv[2], "assoc:%d", &assoc);
    setNum = log2(cacheSize / blockSize / assoc);
  }

  else {
    printf("error\n");
    return 0;
  }

  // replacement policy
  char policy = 'n';

  if (strcmp(argv[3], "lru") == 0) {
    policy = 'l';
  }

  else if (strcmp(argv[3], "fifo") == 0) {
    policy = 'f';
  }

  else {
    printf("error\n");
    return 0;
  }

  char *traceFile = argv[5];

  if(access(traceFile, F_OK) == -1) {
    printf("error");
    return 0;
  }

  char work;
  unsigned long int address;

  FILE * file = fopen(traceFile, "r");

  int i = 0;
  while (fscanf(file, "%*x: %c %lx", &work, &address) == 2) {

    int offsetBits = address & ((1 << offsetNum) - 1);
    int setBits = (address >> offsetNum) & ((1 << setNum) - 1);
    int tagBits = (address >> (offsetNum + setNum));

    // printf("address: %ld\n", address);
    // printf("tagBits, setBits: %d, offsetBits: %d\n", setNum, offsetNum);
    printf("tag: %d, set: %d\n", tagBits, setBits);
    
    if (i == 20) {
      break;
    }
    i++;
  }

  fclose(file);

  // hextobin("804ae19");

}
