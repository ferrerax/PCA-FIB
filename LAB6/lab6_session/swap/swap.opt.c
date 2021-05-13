#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
 
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

    int  n1;
    char *c1;
    char *c1_out;

    c1        = (char *) malloc(count);
    c1_out = (char *) malloc(count);
 
    while ((n1=read(0, c1, count)) > 0)
    {
        int i;
        for (i=0; i<n1-1; i+=2) {
          c1_out[i]     = c1[i+1];
          c1_out[i+1] = c1[i];
        }
        if (i < n1)
          c1_out[i] = c1[i];

        if (write(1, c1_out, n1) < 0) panic("write2");
    }
 
    if ((n1==-1) ) panic("read");
 
    free(c1);
    free(c1_out);
    return 0;
}
