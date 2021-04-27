#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// includes per contar instruccions
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#define _GNU_SOURCE
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <inttypes.h>
#include <sys/types.h>
/////


#define N_ITER 10000131

#define UNROLL_J(i,j) {\
                        C[i][j] = C[i][j] + A[i][0] * B[0][j];\
                        C[i][j] = C[i][j] + A[i][1] * B[1][j];\
                        C[i][j] = C[i][j] + A[i][2] * B[2][j];\
                        C[i][j] = C[i][j] + A[i][3] * B[3][j];\
                      } 

#define UNROLL_I(i) {\
                      UNROLL_J(i,0);\
                      UNROLL_J(i,1);\
                      UNROLL_J(i,2);\
                      UNROLL_J(i,3);\
                    }

#define MULTIPLICA(A, B, C, n_iter) {\
                                     int iter;\
                                     int i,j,k;\
                                     for (iter=0; iter<n_iter; iter++)\
                                     {\
                                       UNROLL_I(0);\
                                       UNROLL_I(1);\
                                       UNROLL_I(2);\
                                       UNROLL_I(3);\
                                     }\
                                  }
static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
                    group_fd, flags);
    return ret;
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
////////////////////////////////////////
// baines rares per contar instruccions
////////////////////////////////////////
		struct perf_event_attr pe;
    long long count;
    int fd;
		memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = PERF_COUNT_HW_INSTRUCTIONS;
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    // Don't count hypervisor events.
    pe.exclude_hv = 1;

    fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening leader %llx\n", pe.config);
        exit(EXIT_FAILURE);
    }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

// macro que ens interesa

    MULTIPLICA(A, B, C, n_iter);

    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
// lectura de instruccions
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    read(fd, &count, sizeof(long long));

    printf("Used %lld instructions\n", count);

/////////////////////////////////////


    print_matriu(C);
         
    return 0;
}
