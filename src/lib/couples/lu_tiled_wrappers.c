#include <stdlib.h>
#include "lu_tiled.h"
#include "lu_tiled_wrappers.h"
#include "common.h"

/* Constructors - destructors */

struct diag_node_params * construct_diag_node_params(double **a, \
        double ***u_inv, double ***l_inv, int B, int k)
{
    struct diag_node_params *lu_p;
    lu_p = malloc(sizeof(struct diag_node_params));
    lu_p->a = a;
    lu_p->k = k;
    lu_p->B = B;
    lu_p->u_inv = u_inv;
    lu_p->l_inv = l_inv;
    return lu_p;
}
struct diag_node_params * destruct_diag_node_params(struct diag_node_params *lu_p)
{
    free(lu_p);
    return NULL;
}

struct LU_node_params * construct_LU_node_params (
        double **a, double *** u_inv, double *** l_inv, \
        int B, int k, int i, double **up_res, double **low_res)
{
    struct LU_node_params *lu_node_p;
    lu_node_p = malloc(sizeof(struct LU_node_params));
    lu_node_p->a = a;
    lu_node_p->k = k;
    lu_node_p->B = B;
    lu_node_p->u_inv = u_inv;
    lu_node_p->l_inv = l_inv;
    lu_node_p->i = i;
    lu_node_p->up_res = up_res;
    lu_node_p->low_res = low_res;
    return lu_node_p;
}
struct LU_node_params * destruct_LU_node_params (struct LU_node_params *lu_node_p)
{
    free(lu_node_p);
    return NULL;
}

struct updating_node_params * construct_updating_node_params(       \
        double **a, int k, int B, int i, int j)
{
    struct updating_node_params *upd_node_p;
    upd_node_p = malloc(sizeof(struct updating_node_params));
    upd_node_p->a = a;
    upd_node_p->k = k;
    upd_node_p->B = B;
    upd_node_p->i = i;
    upd_node_p->j = j;
    return upd_node_p;
}
struct updating_node_params * destruct_updating_node_params(       \
        struct updating_node_params *upd_node_p)
{
    free(upd_node_p);
    return NULL;
}

struct final_node_params * construct_final_node_params(             \
    double **a, int B, int range)
{
    struct final_node_params *fnp;
    fnp = malloc(sizeof(struct final_node_params));
    fnp->a = a;
    fnp->B = B;
    fnp->range = range;
    return fnp;
}
struct final_node_params * destruct_final_node_params(          \
    struct final_node_params *fnp)
{
    free(fnp);
    return NULL;
}


/* Wrapper methods */

void diag_node_wrapper(void *p, int id)
{
    struct diag_node_params *params = (struct diag_node_params *) p;

    double ** a = params->a;
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
    double ** a = params->a;
    int k = params->k;
    int B = params->B;
    int i = params->i;
    debug("%d\n", id);
    mm_upper(a, i*B, k*B, params->up_res, *(params->u_inv), 0, 0, a, i*B, k*B, B, B, B);
}

void lower_node_wrapper(void *p, int id)
{
    struct LU_node_params *params = (struct LU_node_params *) p;
    double ** a = params->a;
    int k = params->k;
    int B = params->B;
    int i = params->i;
    debug("%d\n", id);
    mm_lower(*(params->l_inv), 0, 0, params->low_res, a, k*B, i*B, a, k*B, i*B, B, B, B);
}

void update_node_wrapper(void *p, int id)
{
    struct updating_node_params *params = (struct updating_node_params *) p;
    double ** a = params->a;
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
    double ** a = params->a;
    int B = params->B;
    int range = params->range;
    debug("%d\n", id);
    lu_kernel(a, (range-1)*B, (range-1)*B, B, B);
}
