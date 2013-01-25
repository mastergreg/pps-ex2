/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : lu_tiled.h
* Creation Date : 22-01-2013
* Last Modified : Fri 25 Jan 2013 04:25:44 PM EET
* Created By : Greg Liras <gregliras@gmail.com>
_._._._._._._._._._._._._._._._._._._._._.*/
#ifndef LU_TILED_H
#define LU_TILED_H



void mm(double ** a1,int x_1,int y_1, double ** a2, int x_2, int y_2 , double ** res, int x_3, int y_3, double ** tmp_res, int M, int N, int P, int sign);
void mm_lower(double ** a1,int x_1,int y_1, double **low_res_mine,double ** a2, int x_2, int y_2 , double ** res, int x_3, int y_3, int M, int N, int P);
void mm_upper(double ** a1,int x_1,int y_1, double **up_res_mine,double ** a2, int x_2, int y_2 , double ** res, int x_3, int y_3, int M, int N, int P);
void rectrtri_lower(double ** a, int x_s,int y_s, int N, int M, double ** tmp_res);
void rectrtri_upper(double ** a, int x_s, int y_s, int N, int M, double ** tmp_res);
double ** allocate(int X, int Y);
double ** get_inv_l(double ** a, int xs, int ys, int X, int Y);
double ** get_inv_u(double **a, int xs, int ys, int X, int Y);
void input(double ** a, int X, int Y);
void lu(double ** a, int range, int B);
void lu_kernel(double ** a, int xs, int ys, int X, int Y);
void print(double ** a, int N);
void mm_update(double ** a1, int x_1, int y_1, double ** a2, int x_2, int y_2, double ** res, int x_3, int y_3, int M, int N, int P);

#endif /* LU_TILED_H */
