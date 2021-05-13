#include <stdio.h>
#include <stdlib.h>

#define N 512

int n;

int C[N][N];
int A[N][N];
int B[N][N];


void mult1(int C[][N], int B[][N], int A[][N], int n)
{

  int ii,jj,kk,i,j,k;
  int sum;
  int BLOCK=4;
  if (BLOCK != 0) {
    for ( ii=0 ; ii < n; ii+=BLOCK ){
      for ( jj=0 ; jj < n; jj+=BLOCK ){
        for ( kk=0 ; kk < n; kk+=BLOCK ){
          for ( i=ii ; i < ii+BLOCK ; i++ )
          {
            for ( j=jj ; j < jj+BLOCK; j++ )
            {
              sum=C[i][j];
              for ( k=kk ; k < kk+BLOCK ; k++ )
                 sum+= A[i][k]*B[k][j];
             C[i][j] = sum;
            }
          }
        }
      }
    }
  }

  for ( ; i < n ; i++ )
  {
    for ( ; j < n; j++ )
    {
     sum=C[i][j];
     for ( ; k < n ; k++ )
       sum+= A[i][k]*B[k][j];
     C[i][j] = sum;
    }
  }
}

void init(int A[][N], int n)
{

  int i,j;

  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      A[i][j] = i+j;

}

int main(int argc, char *argv[])
{


  if (argc > 1) n = atoi(argv[1]); else n = N;

  init( A, n);
  init( B, n);
  init( C, n);
  

  mult1(C,B,A,n); 

  write(1, C, sizeof(C));
  return 0;
}
