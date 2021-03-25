
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define N 6000
#define PUNTS 1000

int main(int argc, char *argv[])
{
	unsigned int i, r, j, n, a;
	double d, x, y;
        double cos_memo[PUNTS];
        double sin_memo[PUNTS];
        double xys[PUNTS*2];

	if (argc == 1) n = N; else n = atoi(argv[1]);

	srand(0);

        for (j=0, d=0; j<PUNTS; j++)
        {
                cos_memo[j] = cos(d); 
                sin_memo[j] = sin(d);
                d += 2*M_PI/PUNTS;
        }
	for (i=0; i<n; i++)
	{
		r = rand();
		for (j=0, d=0, a=0; j<PUNTS; j++, a+=2)
		{
			xys[a] = r*cos_memo[j]; 
                        xys[a+1] = r*sin_memo[j];
	//		fwrite(&x, sizeof(x), 1, stdout);
	//		fwrite(&y, sizeof(y), 1, stdout);
			//d += 2*M_PI/PUNTS;
		}
            
		fwrite(xys, sizeof(double), PUNTS*2, stdout);
	}
	return 0;
}
