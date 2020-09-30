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

void print(double **matA, int row, int col) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%0.0lf", matA[i][j]);
    } 
    printf("\n");
  }
}

void cleanUp(double **mat, int row) {
  for (int i = 0; i < row; i++) {
    free(mat[i]);
  }
  free(mat);
}

// main method starts here
int main(int argc, char** argv){

  char *eptr;

  // TRAIN

  // read file
  char *fileName = argv[1];

  FILE* file = fopen(fileName, "r"); 
  char output[100] = "";

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

  double** X = (double **)malloc(rows * sizeof(double *));
  double** Y = (double **)malloc(rows * sizeof(double *));

  int j = 0;
  while (fgets(output, 100, file)) {
    // first two rows are lengths
    X[j] = (double *)malloc(cols * sizeof(double));
    Y[j] = (double *)malloc(1 * sizeof(double));

    char number[100] = "";
    int numberIndex = 0;

    int crntCol = 0;
    int k;

    // skip j = 1 since that is the tab character
    for (k = 0; k < strlen(output); k++) {
      if (output[k] == ',') {
        X[j][crntCol] = strtod(number, &eptr);
        
        // reset
        memset(number,0,strlen(number));
        numberIndex = 0;
        crntCol++;

        // skip over ,
        continue;
      }

      number[numberIndex] = output[k];
      numberIndex++;
    }

    Y[j][0] = strtod(number, &eptr);

    j++;
  }

  fclose(file);

  double** Xt = transposeMatrix(X, rows, cols);
  double** XtX = multiplyMatrix(Xt, X, cols, rows, rows, cols);
  double** XtXinverse = inverseMatrix(XtX, cols);
  double** XtXinverseXt = multiplyMatrix(XtXinverse, Xt, cols, cols, cols, rows);
  // this represents W
  double** XtXinverseXtY = multiplyMatrix(XtXinverseXt, Y, cols, rows, rows, 1);

  // print(Y, rows, 1);
  // printf("\n");

  // print(XtXinverseXtY, cols, 1);

  // TEST

  // read file
  char *fileName2 = argv[2];

  FILE* file2 = fopen(fileName2, "r"); 
  char output2[100] = "";

  int rows2;

  int a = 0; 
  while (fgets(output2, 100, file2)) {
    // read rows
    if (a == 0) {
      rows2 = atoi(output2);
      break;
    }
    a++;
  }

  double** TEST = (double **)malloc(rows2 * sizeof(double *));

  int b = 0;
  while (fgets(output2, 100, file2)) {
    // first two rows are lengths
    TEST[b] = (double *)malloc(cols * sizeof(double));

    char number[100] = "";
    int numberIndex = 0;

    int crntCol = 0;
    for (int k = 0; k < sizeof(output2); k++) {
      if (output2[k] == ',') {
        TEST[b][crntCol] = strtod(number, &eptr);
        
        // reset
        memset(number,0,strlen(number));
        numberIndex = 0;
        crntCol++;

        // skip over ,
        continue;
      }

      number[numberIndex] = output2[k];
      numberIndex++;
    }

    TEST[b][crntCol] = strtod(number, &eptr);

    b++;
  }

  fclose(file2);

  // printf("\n");
  // print(TEST, rows2, cols);

  double** TESTW = multiplyMatrix(TEST, XtXinverseXtY, rows2, cols, cols, 1);

  // printf("\n");
  print(TESTW, rows2, 1);


  // CLEAN UP
  cleanUp(X, rows);
  cleanUp(Y, rows);
  cleanUp(Xt, cols);
  cleanUp(XtX, cols);
  cleanUp(XtXinverse, cols);
  cleanUp(XtXinverseXt, cols);
  cleanUp(XtXinverseXtY, cols);
  cleanUp(TEST, rows2);
  cleanUp(TESTW, rows2);
	
	return 0;
}


// output = r1 X c2
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
  double** result=malloc(r1*sizeof(double*)); 

  for (int y1 = 0; y1 < r1; y1++) {
    result[y1] = (double *)malloc(c2 * sizeof(double));

    for (int x2 = 0; x2 < c2; x2++) {
      double sum = 0;

      for (int y2 = 0; y2 < r2; y2++) {
        sum += matA[y1][y2] * matB[y2][x2];
      }

      result[y1][x2] = sum;
    }

  }
  // your code goes here
  
  return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
  
	double** matTran=malloc(col*sizeof(double*)); 

  for (int x = 0; x < col; x++) {
    matTran[x] = (double *)malloc(row * sizeof(double));

    for (int y = 0; y < row; y++) {
      matTran[x][y] = mat[y][x];
    }
  }
  
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
        matI[x][y] = 1.0;  
      } else {
        matI[x][y] = 0.0;
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