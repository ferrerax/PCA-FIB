#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N_ITER 10000131

#define UNROLL_J(i,j) {\
                        C[i][j] = C[i][j] + A[i][0] * B[0][j];\
                        C[i][j] = C[i][j] + A[i][1] * B[1][j];\
                        C[i][j] = C[i][j] + A[i][2] * B[2][j];\
                        C[i][j] = C[i][j] + A[i][3] * B[3][j];\
                      } 

#define MULTIPLICA(A, B, C, n_iter) {\
                                     int iter;\
                                     int i,j,k;\
                                     for (iter=0; iter<n_iter; iter++)\
                                     {\
                                        for (i = 0; i < 4; i++) \
                                        {\
                                          UNROLL_J(i,0);\
                                          UNROLL_J(i,1);\
                                          UNROLL_J(i,2);\
                                          UNROLL_J(i,3);\
                                        }\
                                     }\
                                  }

void multiplica(int A[4][4], int B[4][4], int C[4][4], unsigned int n_iter)
{
   int iter;
   int i,j,k;

   for (iter=0; iter<n_iter; iter++)
   {
      for (i = 0; i < 4; i++) 
      {
        for (j = 0; j < 4; j++) 
	{
           for (k = 0; k < 4; k++) 
	   {
              C[i][j] = C[i][j] + A[i][k] * B[k][j];
	   }
	}
      }
   }
}

void print_matriu(int C[][4])
{
   int i,j,k;

   for (i = 0; i < 4; i++) 
   {
     for (j = 0; j < 4; j++) 
     {
       printf("%d ", C[i][j]);
     }
     printf("\n");
   }
}

int main(int argc, char *argv[])
{
    int A[4][4], B[4][4], C[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    unsigned int n_iter=N_ITER, i,j;

    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            A[i][j] = rand();
            B[i][j] = rand();
        }
    }

    if (argc > 1) {
	n_iter = atoi(argv[1]); 
    }

    MULTIPLICA(A, B, C, n_iter);

    print_matriu(C);
         
    return 0;
}
