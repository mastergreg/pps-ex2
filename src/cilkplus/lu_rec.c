/***** Recursive LU decomposition *****/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#include <cilk/cilk.h>
#include "common.h"

/* Define the size of a block. */
#define BLOCK 16

static void elem_daxmy(double a, double * x, double * y, int n);
static void block_lu(double ** a, int xs, int ys, int N);
static void block_schur(double ** b, int x1, int y1, double ** a, int x2, int y2, double ** c, int x3, int y3,int N);
void schur(double ** a, int x1, int y1, double ** v, int x2, int y2, double ** w, int x3, int y3, int N);
void lower_solve(double ** a, int x1, int y1, double ** l, int x2, int y2, int N);
static void block_lower_solve(double ** a, int x1, int y1, double ** l, int x2, int y2, int N);
void aux_lower_solve(double ** a1, int x1, int y1, double ** a2, int x2, int y2, double ** l, int x3, int y3, int N);
void upper_solve(double ** a, int x1, int y1, double ** u, int x2, int y2, int N);
static void block_upper_solve(double ** a, int x1, int y1, double ** u, int x2, int y2, int N);
void aux_upper_solve(double ** a1, int x1, int y1, double ** a2, int x2, int y2, double ** u, int x3, int y3, int N);
void lu(double ** a, int xs, int ys, int N);

int main(int argc, char *argv[])
{
    double ** A;
    int N;
    struct timeval ts, tf;
    double time;

    Matrix *mat;

    usage(argc, argv);

    mat = get_matrix(argv[1], 0, CONTINUOUS);
    N = mat->N;
    A = appoint_2D(mat->A, N, N);

    gettimeofday(&ts,NULL);

    cilk_spawn lu(A,0,0,N);

    cilk_sync;
    gettimeofday(&tf,NULL);
    time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;

    printf("Recursive\t%d\t%lf\n", N,time);
//      print(A,N);
    upper_triangularize(N, A);
    print_matrix_2d_to_file(argv[2], N, N, *A);

    
    return 0;
}


/***** Computes y'=y-ax, a scalar, double, x, y vectors of doubles *****/
static void elem_daxmy(double a, double *x, double *y, int n)
{
    for (n--; n >= 0; n--)
        y[n] -= a * x[n];
}

/***** Baseline LU kernel *****/
static void block_lu(double ** a, int xs, int ys, int N)
{
    int i, k;

    /* Factor block. */
    for (k = 0; k < N; k++)
        for (i = k + 1; i < N; i++) {
            a[i+xs][k+ys] /= a[k+xs][k+ys];
            elem_daxmy(a[i+xs][k+ys],&a[k+xs][k+1+ys],&a[i+xs][k+1+ys],N-k-1);
        }
}

/***** Schur's algorithm for base case *****/
static void block_schur(double ** b, int x1, int y1, double ** a, int x2, int y2, double ** c, int x3, int y3,int N)
{
    int i, k;

    /* Compute Schur complement. */
    for (i = 0; i < N; i++)
        for (k = 0; k < N; k++)
            elem_daxmy(a[i+x2][k+y2],&c[k+x3][y3],&b[i+x1][y1],N);
}


/***** Schur's recursive algorithm for 2D matrix multiplication - Computes a=a-v*w *****/
void schur(double ** a, int x1, int y1, double ** v, int x2, int y2, double ** w, int x3, int y3, int N)
{

    int hn;

    /* Check base case. */
    if (N <= BLOCK) {
        block_schur(a, x1, y1, v, x2, y2, w, x3, y3, N);
        return;
    }
    hn = N / 2;


    /* Form Schur complement with recursive calls. */
    cilk_spawn schur(a,x1,y1,v,x2,y2,w,x3,y3,hn);
    cilk_spawn schur(a,x1,y1+hn,v,x2,y2,w,x3,y3+hn,hn);
    cilk_spawn schur(a,x1+hn,y1,v,x2+hn,y2,w,x3,y3,hn);
    cilk_spawn schur(a,x1+hn,y1+hn,v,x2+hn,y2,w,x3,y3+hn,hn);
    cilk_sync;


    cilk_spawn schur(a,x1,y1,v,x2,y2+hn,w,x3+hn,y3,hn);
    cilk_spawn schur(a,x1,y1+hn,v,x2,y2+hn,w,x3+hn,y3+hn,hn);
    cilk_spawn schur(a,x1+hn,y1,v,x2+hn,y2+hn,w,x3+hn,y3,hn);
    cilk_spawn schur(a,x1+hn,y1+hn,v,x2+hn,y2+hn,w,x3+hn,y3+hn,hn);
    cilk_sync;
}



/***** Forward substitution for base case *****/
static void block_lower_solve(double ** a, int x1, int y1, double ** l, int x2, int y2, int N)
{
    int i, k;

    /* Perform forward substitution. */
    for (i = 1; i < N; i++)
        for (k = 0; k < i; k++)
            elem_daxmy(l[i+x2][k+y2],&a[k+x1][y1],&a[i+x1][y1],N);
}

void aux_lower_solve(double ** a1, int x1, int y1, double ** a2, int x2, int y2, double ** l, int x3, int y3, int N)
{

    /* Solve with recursive calls. */
    cilk_spawn lower_solve(a1,x1,y1,l,x3,y3,N);
    cilk_sync;
    cilk_spawn schur(a2,x2,y2,l,x3+N,y3,a1,x1,y1,N);
    cilk_sync;
    cilk_spawn lower_solve(a2,x2,y2,l,x3+N,y3+N,N);
}

/***** Recursive forward substitution - computes a' where la'=a, l lower triangular matrix *****/
void lower_solve(double ** a, int x1, int y1, double ** l, int x2, int y2, int N)
{

    int hn;

    /* Check base case. */
    if (N <= BLOCK) {
        block_lower_solve(a,x1,y1,l,x2,y2,N);
        return;
    }

    hn = N / 2;

    /* Solve with recursive calls. */
    cilk_spawn aux_lower_solve(a,x1,y1,a,x1+hn,y1,l,x2,y2,hn);
    cilk_spawn aux_lower_solve(a,x1,y1+hn,a,x1+hn,y1+hn,l,x2,y2,hn);
    cilk_sync;
}

/***** Forward substitution for base case *****/
static void block_upper_solve(double ** a, int x1, int y1, double ** u, int x2, int y2,int N)
{
    int i, k;

    /* Perform forward substitution. */
    for (i = 0; i < N; i++)
        for (k = 0; k < N; k++) {
            a[i+x1][k+y1]/=u[k+x2][k+y2];
            elem_daxmy(a[i+x1][k+y1],&u[k+x2][k+1+y2],&a[i+x1][k+1+y1],N-k-1);
        }
}


void aux_upper_solve(double ** a1, int x1, int y1, double ** a2, int x2, int y2, double ** u, int x3, int y3, int N)
{
    cilk_spawn upper_solve(a1,x1,y1,u,x3,y3,N);
    cilk_sync;
    cilk_spawn schur(a2,x2,y2,a1,x1,y1,u,x3,y3+N,N);
    cilk_sync;
    cilk_spawn upper_solve(a2,x2,y2,u,x3+N,y3+N,N);
}

/***** Recursive forward substitution - computes a' where a'u=a, u upper triangular matrix *****/
void upper_solve(double ** a, int x1, int y1, double ** u, int x2, int y2, int N)
{
    int hn;

    /* Check base case. */
    if (N <= BLOCK) {
        block_upper_solve(a,x1,y1,u,x2,y2,N);
        return;
    }
    /* Break matrices into 4 pieces. */
    hn = N / 2;

    /* Solve with recursive calls. */
    cilk_spawn aux_upper_solve(a,x1,y1,a,x1,y1+hn,u,x2,y2,hn);
    cilk_spawn aux_upper_solve(a,x1+hn,y1,a,x1+hn,y1+hn,u,x2,y2,hn);
    cilk_sync;
}


/***** Recursive LU decomposition *****/
void lu(double ** a, int xs, int ys, int N)
{
    int hn;

    /* Check base case. */
    if (N <= BLOCK) {
        block_lu(a, xs, ys,N);
        return;
    }

    hn = N / 2;

    /***** Compute LU decomposition on upper left tile A00 recursively*****/
    cilk_spawn lu(a,xs,ys,hn);
    cilk_sync;

    /***** Compute LU decomposition on upper right A01 and lower left A10 tiles via substitution recursively*****/
    cilk_spawn lower_solve(a,xs,ys+hn,a,xs,ys,hn);
    cilk_spawn upper_solve(a,xs+hn,ys,a,xs,ys,hn);
    cilk_sync;

    /***** Update lower right tile A11 recursively*****/
    cilk_spawn schur(a,xs+hn,ys+hn,a,xs+hn,ys,a,xs,ys+hn,hn);
    cilk_sync;
    /***** Compute LU decomposition on lower right tile A11 recursively*****/
    cilk_spawn lu(a,xs+hn,ys+hn,hn);
    cilk_sync;
}
