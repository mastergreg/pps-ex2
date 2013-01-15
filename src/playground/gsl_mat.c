/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : gsl_mat.c
* Creation Date : 11-01-2013
* Last Modified : Fri 11 Jan 2013 03:05:11 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/


#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>



// void rectrtri_upper(double ** a, int x_s,int y_s, int N, int M, double ** tmp_res)
// {
//     gsl_matrix inverse;
//     gsl_permutation p;
//     gsl_linalg_LU_invert(gsl_matrix_view_array(*a, N, M), &p, \
//         gsl_matrix_view_array(*tmp_res, N, M));
// }
// void rectrtri_lower(double ** a, int x_s,int y_s, int N, int M, double ** tmp_res)
// {
//     gsl_matrix inverse;
//     gsl_permutation p;
//     gsl_linalg_LU_invert(gsl_matrix_view_array(*a, N, M), &p, \
//         gsl_matrix_view_array(*tmp_res, N, M));
// }

int main(void)
{
    double A[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    gsl_permutation p;
    gsl_matrix inverse;
    gsl_matrix_view gmv = gsl_matrix_view_array(*A, 3, 3);
    struct gsl_matrix *mp1 = (struct gsl_matrix *) &gmv.matrix;
    gsl_matrix_fprintf(stdout, mp1, "%g");
    gsl_linalg_LU_decomp(mp1, &p, &inverse);

    struct gsl_matrix *mp = (struct gsl_matrix *) &gmv.matrix;
    gsl_matrix_fprintf(stdout, mp, "%g");
    
}
