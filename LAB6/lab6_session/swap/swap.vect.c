#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <tmmintrin.h>
 
void panic(char *miss)
{
        if (errno != 0) perror(miss);
        else fprintf(stderr, "%s\n", miss);
 
        exit(1);
}
 
int main(int argc, char *argv[])
{
    int count = 100;
    if (argc == 2) count = atoi(argv[1]);

    __m128i mask;
    __m128i *c1, *c1_out;

    posix_memalign((void **)&c1,16,count);
    posix_memalign((void **)&c1_out,16,count);
 
    int i;
    // VECTORIAL
    mask = _mm_set_epi8(0xE, 0xF, 0xC, 0xD, 0xA, 0xB, 0x8, 0x9, 0x6, 0x7, 0x4, 0x5, 0x2, 0x3, 0x0, 0x1);
    for(i=0; i < (count>>4); i++)
      c1_out[i] = _mm_shuffle_epi8(c1[i], mask);

    // EPILOG
    for (i=i*16; i<count-1; i+=2) {
      ((char *)c1_out)[i]     = ((char *)c1)[i+1];
      ((char *)c1_out)[i+1]   = ((char *)c1)[i];
    }
    if (i < count)
       ((char *)c1_out)[i] = ((char *)c1)[i];

    write(1,c1_out, count);
 
    free(c1);
    free(c1_out);
    return 0;
}
