#ifndef LU_TILED_WRAPPERS_H
#define LU_TILED_WRAPPERS_H

#include <stdlib.h>

struct diag_node_params {
    double ***a;
    int *** u_inv;
    int *** l_inv;
    int B;
    int k;
}

struct LU_node_params {
    double ***a;
    int *** u_inv;
    int *** l_inv;
    int B;
    int k;
    int i; // Node's index
}

struct updating_node_params {
    double ***a;
    int B;
    int k;
    int i; 
    int j; 
}

struct final_node_params {
    double ***a;
    int B;
    int range;
}

void upper_node_wrapper(struct LU_node_params params);
void lower_node_wrapper(struct LU_node_params params);
void diag_node_wrapper(struct LU_node_params params);
void update_node_wrapper(struct updating_node_params params);
void final_node_wrapper(struct final_node_params params);
#endif /* LU_TILED_WRAPPERS_H */
