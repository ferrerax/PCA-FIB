#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <tmmintrin.h>
 

#define COUNT 64
 
int main(int argc, char *argv[])
{

    __m128i mask;

    __m128i c1[COUNT], c1_out[COUNT];

    mask = _mm_set_epi8(0xE, 0xF, 0xC, 0xD, 0xA, 0xB, 0x8, 0x9, 0x6, 0x7, 0x4, 0x5, 0x2, 0x3, 0x0, 0x1);
    int n1;

    while( (n1 = read(0,c1,COUNT*16)) > 0)
    {
      int i;
      // VECTORIAL
      for(i=0; i < (n1>>4)-3; i+=4) {
        c1_out[i] = _mm_shuffle_epi8(c1[i], mask);
        c1_out[i+1] = _mm_shuffle_epi8(c1[i+1], mask);
        c1_out[i+2] = _mm_shuffle_epi8(c1[i+2], mask);
        c1_out[i+3] = _mm_shuffle_epi8(c1[i+3], mask);
      }
      for(; i < (n1>>4); i++) 
        c1_out[i] = _mm_shuffle_epi8(c1[i], mask);

      // EPILOG ESCALAR
      for (i=i*16; i<n1-1; i+=2) {
        ((char *)c1_out)[i]     = ((char *)c1)[i+1];
        ((char *)c1_out)[i+1]   = ((char *)c1)[i];
      }
      if (i < n1)
         ((char *)c1_out)[i] = ((char *)c1)[i];

      write(1,c1_out, n1);
    }
    return 0;
}
