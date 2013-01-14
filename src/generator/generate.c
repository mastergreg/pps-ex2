/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : generate.c
* Creation Date : 13-11-2012
* Last Modified : Thu 10 Jan 2013 06:00:35 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void usage(int argc, char **argv)
{
    if(argc != 3) {
        printf("Usage: %s <matrix size> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    int i, j;
    int msize;
    FILE *fp;
    double value;
    struct timeval tv;

    usage(argc, argv);


    sscanf(argv[1], "%d", &msize);
    fp = fopen(argv[2], "wb");


    fwrite((void *) &msize, sizeof(int), 1, fp);

    gettimeofday(&tv, NULL);
    srandom(tv.tv_usec);

    for(i = 0; i < msize; i++) {
        for(j = 0; j < msize; j++) {
            while((value = random()/1000000.0) == 0) ;
            fwrite((void *) &value, sizeof(double), 1, fp);
        }
    }

    fclose(fp);

    return 0;
}
