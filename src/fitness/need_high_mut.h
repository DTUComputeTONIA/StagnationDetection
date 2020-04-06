#ifndef EA_NEED_HIGH_MUT_H
#define EA_NEED_HIGH_MUT_H

#include <stdbool.h>

int need_high_mut(bool *x, int n, int m, int block_size);

int need_high_mut_update(bool *x, int n, int m, int block_size, int *flippos);

int is_optimum_need_high_mut(int n, int m, int block_number, int block_size, int fitness);

int is_inlocal_optimum_need_high_mut(int n, int m, int block_number, int block_size, int fitness);

int pre(bool *x, int n, int m);

int suff(bool *x, int n, int m, int block_size);

#endif