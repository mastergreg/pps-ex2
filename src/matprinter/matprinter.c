/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : matprinter.c
* Creation Date : 23-01-2013
* Last Modified : Wed 23 Jan 2013 04:18:32 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/
#include "common.h"

int main(int argc, char **argv)
{
    if(argc != 2) {
        printf("matprinter <matrix file>\n");
        exit(1);
    }
    int N;
    double *A;

    Matrix *m = get_matrix(argv[1], 1, CONTINUOUS);
    N = m->N;
    A = m->A;
    print_matrix_2d(N, N, A);
    free(A);
    return 0;
}
