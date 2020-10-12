#include <stdio.h>
#include <stdlib.h>

int countOnes(int x, int *parity, int *numOfPairs) {
  int crnt = x;
  int count = 0;
  int pairCount = 0;
  int pairLength = 0;

  while (crnt > 0) {
    if ((crnt & 1) == 1) {
      count++;
      pairLength++;

      if (pairLength == 2) {
        pairCount++;
        pairLength = 0;
      }
    }

    else {
      pairLength = 0;
    }

    crnt = crnt >> 1;
  }

  *parity = count % 2;
  *numOfPairs = pairCount;
}

int main(int argc, char* argv[])
{

  int x = atoi(argv[1]);
  int parity = 0;
  int numOfPairs = 0;

  countOnes(x, &parity, &numOfPairs);

  if (parity == 1) {
    printf("Odd-Parity\t%d", numOfPairs);
  } else {
    printf("Even-Parity\t%d", numOfPairs);
  }

}
