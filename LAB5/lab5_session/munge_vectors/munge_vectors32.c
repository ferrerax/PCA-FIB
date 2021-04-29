#include <stdio.h>
#include <stdlib.h>
#include "clock.h"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define N_ITER (1<<2)


void Munge32( void *data, uint32_t size ) {
    int j;
    uint32_t *data32 = (uint32_t*) data;
    uint32_t *data32End = data32 + (size >> 2); /* Divide size by 4. */
    uint8_t *data8 = (uint8_t*) data32End;
    uint8_t *data8End = data8 + (size & 0x00000003); /* Strip upper 30 bits. */
    double cyc, cyc_min;
    
  start_counter();
  for (j=0; j<N_ITER; j++) {
    data32 = (uint32_t*) data;
    while( data32 != data32End ) {
        *data32++ = -*data32;
    }
    data8 = (uint8_t*) data32End;
    while( data8 != data8End ) {
        *data8++ = -*data8;
    }
  }
  cyc = get_counter()/N_ITER;
  printf("%.2f\n", cyc/((float)(data8End-(uint8_t*)data)));
}

#define MAX_DATOS (1024*1024)



int main()
{
  char *buffer = (char *)malloc(MAX_DATOS+64);
  char *buffer_tmp;


  int alignment=0;


  buffer = buffer+(64-((unsigned long int)buffer%64));

  printf("Align.\tCycles/(byte accessed)\n");
  printf("------\t----------------------\n");

  for (alignment=0; alignment<=64; alignment++)
  {
     buffer_tmp = buffer+alignment;
     printf("%d\t",alignment);
     Munge32( (void *)buffer_tmp, MAX_DATOS-alignment );
  }


  return 0;
}

