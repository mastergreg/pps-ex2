#include <stdlib.h>
#include "lu_tiled.h"
#include "lu_tiled_wrappers.h"
#include "common.h"

void diag_node_wrapper(void *p, int id)
{
    struct diag_node_params *params = (struct diag_node_params *) p;

    double ** a = *(params->a);
    int k = params->k;
    int B = params->B;


    debug("%d\n", id);



    lu_kernel(a, k*B, k*B, B, B);
    *(params->l_inv) = get_inv_l(a, k*B, k*B, B, B);
    *(params->u_inv) = get_inv_u(a, k*B, k*B, B, B);
}

void upper_node_wrapper(void *p, int id)
{
    struct LU_node_params *params = (struct LU_node_params *) p;
    double ** a = *(params->a);
    int k = params->k;
    int B = params->B;
    int i = params->i;
    debug("%d\n", id);
    mm_upper(a, i*B, k*B, *(params->u_inv), 0, 0, a, i*B, k*B, B, B, B);
}

void lower_node_wrapper(void *p, int id)
{
    struct LU_node_params *params = (struct LU_node_params *) p;
    double ** a = *(params->a);
    int k = params->k;
    int B = params->B;
    int i = params->i;
    debug("%d\n", id);
    mm_lower(*(params->l_inv), 0, 0, a, k*B, i*B, a, k*B, i*B, B, B, B);
}

void update_node_wrapper(void *p, int id)
{
    struct updating_node_params *params = (struct updating_node_params *) p;
    double ** a = *(params->a);
    int k = params->k;
    int B = params->B;
    int i = params->i;
    int j = params->j;
    debug("%d\n", id);
    mm_update(a, i*B, k*B, a, k*B, j*B, a, i*B, j*B, B, B, B);
}


void final_node_wrapper(void *p, int id)
{
    struct final_node_params *params = (struct final_node_params *) p;
    double ** a = *(params->a);
    int B = params->B;
    int range = params->range;
    debug("%d\n", id);
    lu_kernel(a, (range-1)*B, (range-1)*B, B, B);
}
