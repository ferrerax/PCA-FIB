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

    __m128i a0;
    __m128i a1;
    __m128i a2;
    __m128i a3;

    //__m128i c1[COUNT], c1_out[COUNT];
    char c1[COUNT] __attribute__((aligned(16)));
    char c1_out[COUNT] __attribute__((aligned(16)));

    mask = _mm_set_epi8(0xE, 0xF, 0xC, 0xD, 0xA, 0xB, 0x8, 0x9, 0x6, 0x7, 0x4, 0x5, 0x2, 0x3, 0x0, 0x1);
    int n1;

    while( (n1 = read(0,c1,COUNT*16)) > 0)
    {
      int i;
      // VECTORIAL
      for(i=0; i < n1-63; i+=16*4) {
	      
        a0 = _mm_load_si128((__m128i *)&c1[i]);
        a1 = _mm_load_si128((__m128i *)&c1[i+16]);
        a2 = _mm_load_si128((__m128i *)&c1[i+16*2]);
        a3 = _mm_load_si128((__m128i *)&c1[i+16*3]);

	a0 = _mm_shuffle_epi8(a0, mask);
        a1 = _mm_shuffle_epi8(a1, mask);
        a2 = _mm_shuffle_epi8(a2, mask);
        a3 = _mm_shuffle_epi8(a3, mask);

	_mm_store_si128((__m128i *)&c1_out[i],   a0);
	_mm_store_si128((__m128i *)&c1_out[i+16*1], a1);
	_mm_store_si128((__m128i *)&c1_out[i+16*2], a2);
	_mm_store_si128((__m128i *)&c1_out[i+16*3], a3);
      }
      
      //Epilog vectorial
      for(; i < n1-15; i+=16) {
        a0 = _mm_load_si128((__m128i *)&c1[i]);
        a0 = _mm_shuffle_epi8(a0, mask);
	_mm_store_si128((__m128i *)&c1_out[i], a0);
      }

      // EPILOG ESCALAR
      for (; i<n1-1; i+=2) {
        c1_out[i]     = c1[i+1];
        c1_out[i+1]   = c1[i];
      }
      if (i < n1)
         c1_out[i] = c1[i];

      write(1, c1_out, n1);
    }
    return 0;
}
