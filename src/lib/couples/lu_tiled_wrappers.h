#ifndef LU_TILED_WRAPPERS_H
#define LU_TILED_WRAPPERS_H

#include <stdlib.h>

/* Data structures */

struct diag_node_params {
    double **a;
    double *** u_inv;
    double *** l_inv;
    int B;
    int k;
};

struct LU_node_params {
    double **a;
    double *** u_inv;
    double *** l_inv;
    int B;
    int k;
    int i; // Node's index
    double **up_res;
    double **low_res;
};

struct updating_node_params {
    double **a;
    int B;
    int k;
    int i;
    int j;
};

struct final_node_params {
    double **a;
    int B;
    int range;
};

/* Constructors - destructors */

struct diag_node_params * construct_diag_node_params(double **a, \
        double ***u_inv, double ***l_inv, int B, int k);
struct diag_node_params * destruct_diag_node_params(struct diag_node_params *lu_p);

struct LU_node_params * construct_LU_node_params (
    double **a, double *** u_inv, double *** l_inv, \
    int B, int k, int i, double **up_res, double **low_res);
struct LU_node_params * destruct_LU_node_params (struct LU_node_params *lu_node_p);

struct updating_node_params * construct_updating_node_params(       \
        double **a, int k, int B, int i, int j);
struct updating_node_params * destruct_updating_node_params(       \
        struct updating_node_params *upd_node_p);

struct final_node_params * construct_final_node_params(             \
    double **a, int B, int range);
struct final_node_params * destruct_final_node_params(          \
    struct final_node_params *fnp);

/* Wrapper methods */

void diag_node_wrapper(void *p, int id);
void upper_node_wrapper(void *p, int id);
void lower_node_wrapper(void *p, int id);
void update_node_wrapper(void *p, int id);
void final_node_wrapper(void *p, int id);
#endif /* LU_TILED_WRAPPERS_H */
