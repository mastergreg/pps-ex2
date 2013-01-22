#include <stdlib.h>

void diag_node_wrapper(struct diag_node_params params)
{
    double ** a = (params.a)*;
    int k = params.k;
    int B = params.B;

    lu_kernel(a, k*B, k*B, B, B);
    l_inv = get_inv_l(a, k*B, k*B, B, B);
    u_inv = get_inv_u(a, k*B, k*B, B, B);
}

void upper_node_wrapper(struct LU_node_params params)
{
    double ** a = (params.a)*;
    int k = params.k;
    int B = params.B;
    int i = params.i;
    mm_upper(a, i*B, k*B, u_inv, 0, 0, a, i*B, k*B, B, B, B)
}

void lower_node_wrapper(struct LU_node_params params)
{
    double ** a = (params.a)*;
    int k = params.k;
    int B = params.B;
    int i = params.i;
    mm_lower(l_inv, 0, 0, a, k*B, i*B, a, k*B, i*B, B, B, B);
}

void update_node_wrapper(struct updating_node_params params)
{
    double ** a = (params.a)*;
    int k = params.k;
    int B = params.B;
    int i = params.i;
    int j = params.j;
    mm_update(a, i*B, k*B, a, k*B, j*B, a, i*B, j*B, B, B, B);
}


void final_node_wrapper(struct final_node_params params)
{
    double ** a = (params.a)*;
    int B = params.B;
    int range = params.range;
    lu_kernel(a, (range-1)*B, (range-1)*B, B, B);
}
