
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
	double d;
	double memo_cos[PUNTS], memo_sin[PUNTS];
	double xy[PUNTS*2];
	if (argc == 1) n = N; else n = atoi(argv[1]);


	for(i=0, d=0; i<PUNTS; i++){
			memo_cos[i] = cos(d);
			memo_sin[i] = sin(d);
			d += 2*M_PI/PUNTS;
	}	

	srand(0);
	for (i=0; i<n; i++)
	{
		r = rand();
		for (j=0, a=0; j<PUNTS; j++, a+=2)
		{
			xy[a] = r*memo_cos[j];
			xy[a+1] = r*memo_sin[j];
		}
		fwrite(xy, sizeof(double), 2*PUNTS, stdout);
	}
	return 0;
}
