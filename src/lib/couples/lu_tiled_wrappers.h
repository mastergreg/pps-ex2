#ifndef LU_TILED_WRAPPERS_H
#define LU_TILED_WRAPPERS_H

#include <stdlib.h>

struct diag_node_params {
    double ***a;
    double *** u_inv;
    double *** l_inv;
    int B;
    int k;
};

struct LU_node_params {
    double ***a;
    double *** u_inv;
    double *** l_inv;
    int B;
    int k;
    int i; // Node's index
    double ***up_res;
    double ***low_res;
};

struct updating_node_params {
    double ***a;
    int B;
    int k;
    int i;
    int j;
};

struct final_node_params {
    double ***a;
    int B;
    int range;
};

void diag_node_wrapper(void *p, int id);
void upper_node_wrapper(void *p, int id);
void lower_node_wrapper(void *p, int id);
void update_node_wrapper(void *p, int id);
void final_node_wrapper(void *p, int id);
#endif /* LU_TILED_WRAPPERS_H */
