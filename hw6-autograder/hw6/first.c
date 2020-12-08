#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int reads;
int writes;
int hits;
int misses;
long int globalIndex;

struct Node {
  int valid;
  long int tag;
  long int index;
};

int cmpprefix(const char *pre, const char *str) {
  return strncmp(pre, str, strlen(pre));
}

long int getGlobalIndex() {
  return globalIndex++;
}

void writeDirect(struct Node *cache, int setBits, long tagBits) {
  // printf("%d == %d\n", cache[setBits].tag, tagBits);
  writes++;

  if (cache[setBits].valid == 1 && cache[setBits].tag == tagBits) {
    // exsists in cache already
    hits++;
  } 

  else {
    // does not exsist in cache
    reads++;
    misses++;
    cache[setBits].valid = 1;
    cache[setBits].tag = tagBits;
  }
}

void readDirect(struct Node *cache, int setBits, long tagBits) {
  if (cache[setBits].valid == 1 && cache[setBits].tag == tagBits) {
    // exsists in cache already
    hits++;
  } 

  else {
    // does not exsist in cache
    misses++;
    reads++;
    cache[setBits].valid = 1;
    cache[setBits].tag = tagBits;
  }
}

void nAssocWrite(struct Node *cache, int setBits, long tagBits, int assoc, char policy) {
  writes++;

  int topOfSet = setBits * assoc;
  int bottomOfSet = (setBits + 1) * assoc;

  long int oldest = -1;
  for (int r = topOfSet; r < bottomOfSet; r++) {
    if (oldest == -1 || cache[r].index < oldest) {
      oldest = cache[r].index;
    }

    if (cache[r].tag == tagBits) {
      // exsists in cache already
      if (policy == 'l') {
        cache[r].index = getGlobalIndex();
      }
      hits++;
      return;
    }
  }

  // does not exsist in cache
  reads++;
  misses++;

  for (int r = topOfSet; r < bottomOfSet; r++) {
    if (cache[r].index == oldest) {
      cache[r].index = getGlobalIndex();
      cache[r].tag = tagBits;
      break;
    }
  }
}

void nAssocRead(struct Node *cache, int setBits, long tagBits, int assoc, char policy) {
  int topOfSet = setBits * assoc;
  int bottomOfSet = (setBits + 1) * assoc;

  long int oldest = -1;
  for (int r = topOfSet; r < bottomOfSet; r++) {
    if (oldest == -1 || cache[r].index < oldest) {
      oldest = cache[r].index;
    }

    if (cache[r].tag == tagBits) {
      // exsists in cache already
      if (policy == 'l') {
        cache[r].index = getGlobalIndex();
      }
      hits++;
      return;
    }
  }

  // does not exsist in cache
  misses++;
  reads++;

  for (int r = topOfSet; r < bottomOfSet; r++) {
    if (cache[r].index == oldest) {
      cache[r].index = getGlobalIndex();
      cache[r].tag = tagBits;
      break;
    }
  }
}

int main(int argc, char* argv[])
{

  // is the total size of the cache in bytes. 
  // This number should be a power of 2
  globalIndex = 1;
  long cacheSize = atoi(argv[1]);
  long assoc;
  long setNum;
  long cacheRows;

  long blockSize = atoi(argv[4]);

  // | Tag | Set | Offset |
  long offsetNum = log2(blockSize);

  //  simulate a direct mapped cache
  if (strcmp(argv[2], "direct") == 0) {
    assoc = 1;
    cacheRows = cacheSize / blockSize;
    setNum = log2(cacheSize / blockSize);
  } 

  // simulate a fully associative cache
  else if (strcmp(argv[2], "assoc") == 0) {
    assoc = cacheSize / blockSize;
    cacheRows = cacheSize / blockSize;
    setNum = log2(cacheSize / blockSize / assoc);
  } 
  
  // simulate an n-way associative cache. n will be a power of 2
  else if(cmpprefix("assoc:", argv[2]) == 0) {
    sscanf(argv[2], "assoc:%ld", &assoc);
    cacheRows = cacheSize / blockSize;
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
  long int address;

  FILE * file = fopen(traceFile, "r");

  // direct map cache 
  struct Node cache[cacheRows];
  
  for (int r = 0; r < cacheRows; r++) {
    struct Node row;

    row.index = -1;
    row.tag = -1;
    row.valid = 0;

    cache[r] = row;
  }

  while (fscanf(file, "%*x: %c %lx", &work, &address) == 2) {
    // int offsetBits = address & ((1 << offsetNum) - 1);
    long int setBits = (address >> offsetNum) & ((1 << setNum) - 1);
    long int tagBits = (address >> (offsetNum + setNum));

    // direct
    if (strcmp(argv[2], "direct") == 0) {
      if (work == 'R') {
        readDirect(cache, setBits, tagBits);
      } 
      
      else if (work == 'W') {
        writeDirect(cache, setBits, tagBits);
      }
    }

    // associative
    else {
      if (work == 'R') {
        nAssocRead(cache, setBits, tagBits, assoc, policy);
      } 
      
      else if (work == 'W') {
        nAssocWrite(cache, setBits, tagBits, assoc, policy);
      }
    }
  }

  fclose(file);

  printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", reads, writes, hits, misses);
}
