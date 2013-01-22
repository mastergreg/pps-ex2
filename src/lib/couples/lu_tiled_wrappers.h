#include <stdlib.h>

double ** l_inv;
double ** u_inv;

struct diag_node_params {
    double **a;
    int B;
    int k;
}

struct LU_node_params {
    double **a;
    int B;
    int k;
    int i; // Node's index
}

struct updating_node_params {
    double **a;
    int B;
    int k;
    int i; // Node's index
}

struct final_node_params {
    double **a;
    int B;
    int range;
}

void upper_node_wrapper(struct LU_node_params params);
void lower_node_wrapper(struct LU_node_params params);
void diag_node_wrapper(struct LU_node_params params);
void update_node_wrapper(struct updating_node_params params);
void final_node_wrapper(struct final_node_params params);
