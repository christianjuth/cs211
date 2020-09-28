/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);

void print(double **matA, int dimension) {
  for (int i = 0; i < dimension; i++) {
    for (int j = 0; j < dimension; j++) {
      printf("%0.2lf\t", matA[i][j]);
    } 
    printf("\n");
  }
}



// main method starts here
int main(int argc, char** argv){

  // read file
  char *fileName = argv[1];

  FILE* file = fopen(fileName, "r"); 
  char output[100] = "";

  // if(access(fileName, F_OK) == -1) {
  //   printf("error");
  //   return 0;
  // }

  int cols;
  int rows;

  int i = 0; 
  while (fgets(output, 100, file)) {
    // read cols
    if (i == 0) {
      cols = atoi(output);
    }

    // read rows
    if (i == 1) {
      rows = atoi(output);
      break;
    }
    i++;
  }

  double** data = (double **)malloc(rows * sizeof(double *));

  int j = 0;
  while (fgets(output, 100, file)) {
    // first two rows are lengths
    data[j] = (double *)malloc(cols * sizeof(double));

    char number[48] = "";
    int numberIndex = 0;

    int crntCol = 0;
    int k;
    // skip j = 1 since that is the tab character
    for (k = 0; k < sizeof(output); k++) {
      if (output[k] == ',') {
        data[j][crntCol] = atoi(number);
        
        // reset
        number[0] = 0;
        numberIndex = 0;
        crntCol++;

        // skip over ,
        continue;
      }

      number[numberIndex] = output[k];
      numberIndex++;
    }

    j++;
  }

  fclose(file); 

  // test
  print(data, 3);
  printf("\n");

  double** invert = inverseMatrix(data, 3);
  print(invert, 3);

  // cleanup
  free(data);
  // free(invert);
	
	return 0;
}



double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
  double** result=malloc(r1*sizeof(double*)); 
  
  // your code goes here
  
  return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
  
	double** matTran=malloc(col*sizeof(double*)); 
    
  // your code goes here
  
  return matTran;        
}


double** inverseMatrix(double **matA, int dimension)
{

  double** matI=malloc(dimension*sizeof(double*)); 

  // initialize
  for (int x = 0; x < dimension; x++) {
    matI[x] = (double *)malloc(dimension * sizeof(double));

    for (int y = 0; y < dimension; y++) {
      if (y == x) {
        matI[x][y] = 1;  
      } else {
        matI[x][y] = 0;
      }
    }
  }

  for (int p = 0; p < dimension; p++) {
    double f = matA[p][p];

    // divide matA and matI
    for (int r = 0; r < dimension; r++) {
      matA[p][r] = matA[p][r] / f;
      matI[p][r] = matI[p][r] / f;
    }

    for (int i = p + 1; i < dimension; i++) {
      double crnt = matA[i][p];

      for (int r = 0; r < dimension; r++) {
        matA[i][r] = matA[i][r] - (matA[p][r] * crnt);
        matI[i][r] = matI[i][r] - (matI[p][r] * crnt);
      }
    }
  }

  for (int p = dimension - 1; p >= 0; p--) {
    for (int i = p - 1; i >= 0; i--) {
      double crnt = matA[i][p];

      for (int r = 0; r < dimension; r++) {
        matA[i][r] = matA[i][r] - (matA[p][r] * crnt);
        matI[i][r] = matI[i][r] - (matI[p][r] * crnt);
      }
    }
  }
    
	return matI;
}


