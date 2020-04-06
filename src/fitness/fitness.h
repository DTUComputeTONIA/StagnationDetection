#ifndef MUTATION_H_INCLUDED
#define MUTATION_H_INCLUDED

#include <stdbool.h>

int eval_jump(bool *x, int n, int k);

int eval_one_max(bool *x, int n);

int eval_onemax_from_jump(int n, int jump_size, int jump_value);

int eval_jump_update(bool *x, int n, int jump_size, int *flippos, int fitness);

int is_optimum_jump_function(int n, int jump, int fitness);

#endif