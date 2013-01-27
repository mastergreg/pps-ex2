/***** Tiled LU decomposition*****/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "common.h"

#include "task_config.h"
#include "lu_tiled.h"
#include "lu_tiled_wrappers.h"


void execute(struct_task *t, int id)
{
    (*(t->func))(t->args, id);
}

struct_task * set_task(void *func, void *args)
{
    struct_task *t = malloc(sizeof(struct task));
    t -> func = func;
    t -> args = args;
    return t;
}

void execute_node(struct_task_node *TASK_GRAPH, int id)
{
    struct_task_node *tn = &TASK_GRAPH[id];
    int i;
    struct_task_node *child;

    execute(tn->mtask, tn->id);

    /* DEBUG
    if (tn->dependencies_count!=0) {
        printf("  CONCURRENCY ISSUE: Node %d: dependencies count is %d after execute\n", tn->id, tn->dependencies_count);
    }
    /DEBUG */

    for(i = 0; i < tn->children_count; ++i) {
        child = &(TASK_GRAPH[tn->children[i]]);

        /* DEBUG 
        if (child->dependencies_count < 1) {
            printf("  CONCURRENCY ISSUE Node %d: parent %d wants to decrease my dependencies_count from %d\n" ,child->id, tn->id, child->dependencies_count);
        }
        /DEBUG */

        if(g_atomic_int_dec_and_test(&(child->dependencies_count))) {
            cilk_spawn execute_node(TASK_GRAPH, child->id);
        }
    }
    cilk_sync;
}

int min(int x, int y)
{
    return x<y?x:y;
}

int main(int argc, char *argv[])
{
    double ** A;
    int B, num_blocks;
    int N;
    struct timeval ts, tf;
    double time;

    Matrix *mat;
    tiled_usage(argc, argv);

    mat = get_matrix(argv[1], 0, CONTINUOUS);
    N = mat->N;
    A = appoint_2D(mat->A, N, N);

    /* Since we have statically 4 blocks, block size arg is ignored */
    num_blocks = 4;
    B = N/num_blocks;

    if (N%B!=0 || B==1) {
        debug("\t Grid is not a multiple of Block size \n");
        exit(0);
    }

    lu(A,num_blocks,B);
    printf("Graph Populated, Executing\n");

    gettimeofday(&ts,NULL);
    execute_node(TASK_GRAPH_A, 0);
    gettimeofday(&tf,NULL);

    time=(tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
    printf("Tiled\t%d\t%lf\tBlock Size: %d\n", N,time,B);
    upper_triangularize(N, A);
    print_matrix_2d_to_file(argv[2], N, N, *A);
    return 0;
}

/**** Tiled LU decomposition *****/
void lu(double **a, int range, int B)
{
    int i,j,k;
    double *** up_res_arr, *** low_res_arr;
    struct diag_node_params *lu_p;
    struct LU_node_params *lu_node_p;
    struct updating_node_params *upd_node_p;
    struct final_node_params *fnp;

    up_res_arr = malloc(range*sizeof(double **));
    low_res_arr = malloc(range*sizeof(double **));
    for(i = 0; i < range; i++) {
        low_res_arr[i]=allocate(B,(range-1)*B);
        up_res_arr[i]=allocate((range-1)*B,B);
    }

    double *** l_inv_arrs = malloc(range*sizeof(double **));
    double *** u_inv_arrs = malloc(range*sizeof(double **));

    int node_counter = 0;

    for (k=0; k<range-1; k++) {

        /* set diagonal nodes (lu_kernel and lu_inv calculation) */
        lu_p = construct_diag_node_params(a, &(u_inv_arrs[k]), &(l_inv_arrs[k]), B, k);
        TASK_GRAPH_A[node_counter++].mtask = set_task(diag_node_wrapper, (void *) lu_p);

        /* set upper and lower nodes (mm_upper and mm_lower)*/
        for (i=k+1; i<range; i++) {
            lu_node_p = construct_LU_node_params(a, &(u_inv_arrs[k]), &(l_inv_arrs[k]),
                    B, k, i, up_res_arr[k], low_res_arr[k]);
            TASK_GRAPH_A[node_counter++].mtask = set_task(upper_node_wrapper, (void *) lu_node_p);
        }
        for (i=k+1; i<range; i++) {
            lu_node_p = construct_LU_node_params(a, &(u_inv_arrs[k]), &(l_inv_arrs[k]),
                    B, k, i, up_res_arr[k], low_res_arr[k]);
            TASK_GRAPH_A[node_counter++].mtask = set_task(lower_node_wrapper, (void *) lu_node_p);
        }

        /* set updating nodes (mm_update) */
        for (i=k+1; i<range; i++) {
            for (j=k+1; j<range; j++) {
                upd_node_p = construct_updating_node_params(a, k, B, i, j);
                TASK_GRAPH_A[node_counter++].mtask = set_task(update_node_wrapper, (void *) upd_node_p);
            }
        }
    }

    /* set final node (lu_kernel) */
    fnp = construct_final_node_params(a, B, range);
    TASK_GRAPH_A[node_counter++].mtask = set_task(final_node_wrapper, (void *) fnp);
}

/***** Baseline LU Kernel *****/
void lu_kernel(double ** a, int xs,int ys, int X, int Y)
{
    int i,j,k;
    double l;
    for (k=0; k<min(X,Y); k++)
        for (i=k+1; i<X; i++) {
            l=a[i+xs][k+ys]=a[i+xs][k+ys]/a[k+xs][k+ys];
            for (j=k+1; j<Y; j++)
                a[i+xs][j+ys]-=l*a[k+xs][j+ys];
        }
}

/***** Computes the inverted L^(-1) matrix of upper diagonal block using rectrtri_lower() *****/
double ** get_inv_l(double ** a, int xs, int ys, int X, int Y)
{
    double ** l_inv=allocate(X,Y);
    double ** tmp_res=allocate(X,Y);
    int i,j;
    for (i=0; i<X; i++)
        for (j=0; j<Y; j++) {
            if (i>j)
                l_inv[i][j]=a[i+xs][j+ys];
            else if (i==j)
                l_inv[i][j]=1.0;
            else l_inv[i][j]=0;
        }
    rectrtri_lower(l_inv,0,0,X,Y,tmp_res);
    free(tmp_res);
    return l_inv;

}

/***** Computer the inverted U^(-1) matrix of upper diagonal block using rectrtri_upper() *****/
double ** get_inv_u(double **a, int xs, int ys, int X, int Y)
{
    double ** u_inv=allocate(X,Y);
    double ** tmp_res=allocate(X,Y);
    int i,j;
    for (i=0; i<X; i++)
        for (j=0; j<Y; j++) {
            if (i<=j)
                u_inv[i][j]=a[i+xs][j+ys];
            else u_inv[i][j]=0;
        }
    rectrtri_upper(u_inv,0,0,X,Y,tmp_res);
    free(tmp_res);
    return u_inv;
}


double ** allocate(int X,int Y)
{
    double ** array;
    array=(double**)calloc(X,sizeof(double*));
    int i;
    for (i=0; i<X; i++)
        array[i]=(double*)calloc(Y,sizeof(double));
    return array;
}

/***** Matrix multiplication of an upper triangular matrix with a full matrix *****/
void mm_upper(double ** a1, int x_1, int y_1, double **up_res, double ** a2, int x_2, int y_2, double ** res, int x_3, int y_3, int M, int N, int P)
{
    int i,j,k;
    double sum=0;
    for (i=0; i<M; i++)
        for (j=0; j<P; j++) {
            for (k=0; k<=j; k++)
                sum=sum+a1[i+x_1][k+y_1]*a2[k+x_2][j+y_2];
            up_res[i+x_3-M][j]=sum;
            sum=0;
        }
    for (i=0; i<M; i++)
        for (j=0; j<P; j++)
            res[i+x_3][j+y_3]=up_res[i+x_3-M][j];
}

/***** Matrix multiplication of a lower triangular matrix with a full matrix *****/
void mm_lower(double ** a1, int x_1, int y_1, double **low_res, double ** a2, int x_2, int y_2, double ** res, int x_3, int y_3, int M, int N, int P)
{
    int i,j,k;
    double sum=0;

    for (i=0; i<M; i++)
        for (j=0; j<P; j++) {
            for (k=0; k<=i; k++)
                sum=sum+a1[i+x_1][k+y_1]*a2[k+x_2][j+y_2];
            low_res[i][j+y_3-P]=sum;
            sum=0;
        }
    for (i=0; i<M; i++)
        for (j=0; j<P; j++)
            res[i+x_3][j+y_3]=low_res[i][j+y_3-P];
}

void mm_update(double ** a1, int x_1, int y_1, double ** a2, int x_2, int y_2, double ** res, int x_3, int y_3, int M, int N, int P)
{
    int i,j,k;
    double sum=0;

    for (i=0; i<M; i++)
        for (j=0; j<P; j++) {
            for (k=0; k<N; k++)
                sum+=a1[i+x_1][k+y_1]*a2[k+x_2][j+y_2];
            res[i+x_3][j+y_3]-=sum;
            sum=0;
        }
}

/***** Matrix multiplication: sign=0 --> res=a1*a2, sign=1 --> res=-a1*a2, op = 0 --> res=a1*a2, op = 1 --> res=res+a1*a2  *****/
void mm(double ** a1,int x_1,int y_1, double ** a2, int x_2, int y_2 , double ** res, int x_3, int y_3, double ** tmp_res, int M, int N, int P, int sign)
{
    int i,j,k;
    double sum=0;

    for (i=0; i<M; i++)
        for (j=0; j<P; j++) {
            for (k=0; k<N; k++)
                sum=sum+a1[i+x_1][k+y_1]*a2[k+x_2][j+y_2];
            tmp_res[i+x_3][j+y_3]=sum;
            sum=0;
        }

    for (i=0; i<M; i++)
        for (j=0; j<P; j++) {
            for (k=0; k<N; k++)
                sum=sum-a1[i+x_1][k+y_1]*a2[k+x_2][j+y_2];
            tmp_res[i+x_3][j+y_3]=sum;
            sum=0;
        }

    if (sign==0) {
        for (i=0; i<M; i++)
            for (j=0; j<P; j++)
                res[i+x_3][j+y_3]=tmp_res[i+x_3][j+y_3];
    } else if (sign==1) {
        for (i=0; i<M; i++)
            for (j=0; j<P; j++)
                res[i+x_3][j+y_3]=-tmp_res[i+x_3][j+y_3];
    }
}


/**** Recursive matrix inversion of a lower triangular matrix *****/
void rectrtri_lower(double ** a, int x_s,int y_s, int N, int M, double ** tmp_res)
{
    int n,m;
    if (N==1 && M==1)
        a[x_s][y_s]=1.0/a[x_s][y_s];
    else {
        n=N/2;
        m=M/2;
        rectrtri_lower(a,x_s,y_s,n,m, tmp_res);
        rectrtri_lower(a,x_s+n,y_s+m,N-n,M-m, tmp_res);
        mm(a,x_s+n,y_s,a,x_s,y_s,a,x_s+n,y_s,tmp_res,N-n,m,m,0);
        mm(a,x_s+n,y_s+m,a,x_s+n,y_s,a,x_s+n,y_s,tmp_res,N-n,M-m,m,1);
    }
}

/***** Recursive matrix inversion of an upper triangular matrix *****/
void rectrtri_upper(double ** a, int x_s,int y_s, int N, int M, double ** tmp_res)
{
    int n,m;
    if (N==1 && M==1)
        a[x_s][y_s]=1.0/a[x_s][y_s];
    else {
        n=N/2;
        m=M/2;

        rectrtri_upper(a,x_s,y_s,n,m, tmp_res);
        rectrtri_upper(a,x_s+n,y_s+m,N-n,M-m, tmp_res);
        mm(a,x_s,y_s+m,a,x_s+n,y_s+m,a,x_s,y_s+m,tmp_res,n,M-m,M-m,0);
        mm(a,x_s,y_s,a,x_s,y_s+m,a,x_s,y_s+m,tmp_res, n,m,M-m,1);
    }

}

