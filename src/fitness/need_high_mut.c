#include "need_high_mut.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>


#include <stdbool.h>
#include "../algorithms/utils.h"

int i;

int local_eval_one_max(bool *x, int n) {
    int count = 0;
    for (i = 0; i < n; ++i)
        if (x[i] == true) count++;
    return count;
}

int one_bits;


int pre(bool *x, int n, int m) {
    int pre = 0;
    bool observing_zero = false;
    for (i = 0; i < n - m; i++) {
        if (x[i] == 1) {
            if (observing_zero == false) {
                pre++;
            } else {
                return -1;
            }
        } else {
            observing_zero = true;
        }
    }
    return pre;
}

int suff(bool *x, int n, int m, int block_size) {
    int suff = 0;
    bool observing_inactive = false;

    for (i = n - m; i < n; i += block_size) {

        one_bits = 0;

        for (int j = 0; j < block_size && i + j < n; j++) {
            if (x[i + j] == 1)
                one_bits++;
        }
        if (one_bits == 0) {
            observing_inactive = true;
        } else if (one_bits == 2) {
            if (observing_inactive == false) {
                suff++;
            } else
                return -1;
        } else {
            return -1;
        }
    }
    return suff;
}

int pre_value, suff_value;
bool x_valid;
int threshold;

int need_high_mut(bool *x, int n, int m, int block_size) {

    pre_value = pre(x, n, m);
    suff_value = suff(x, n, m, block_size);

    x_valid = (pre_value != -1 && suff_value != -1);
    threshold = 9.0 * (n - m) / 10;

    if (pre_value <= threshold && x_valid) {
        return n * n * suff_value + pre_value;
    } else if (pre_value > threshold && x_valid) {
        return n * n * m + pre_value + suff_value - n - 1;
    } else {
        return -local_eval_one_max(x, n);
    }
}

bool y[100000];

int need_high_mut_update(bool *x, int n, int m, int block_size, int *flippos) {
    for (i = 0; i < n; ++i) {
        y[i] = x[i];
    }
    flip_bits(y, flippos);

    return need_high_mut(y, n, m, block_size);
}

int is_optimum_need_high_mut(int n, int m, int block_number, int block_size, int fitness) {
    int max_suff = block_number;

    long long best_possible_fitness = (long long) n * n * max_suff + 9 * (n - m) / 10;
    if (fitness == best_possible_fitness)

        return true;
    return false;
}

int is_inlocal_optimum_need_high_mut(int n, int m, int block_number, int block_size, int fitness) {
    int max_suff = block_number;
    int max_pre = n - m;
    long long local_opt = (long long) n * n * m + max_pre + max_suff - n - 1;

    if (fitness == local_opt)
        return true;
    return false;
}