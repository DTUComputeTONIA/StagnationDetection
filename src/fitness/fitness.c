#include <stdbool.h>
#include <stdio.h>

int eval_one_max(bool *x, int n) {
    int i;
    int count = 0;
    for (i = 0; i < n; ++i)
        if (x[i] == true) count++;
    return count;
}

int eval_jump_with_onemax(int n, int k, int one_max) {
    if ((one_max <= n - k) || (one_max == n))
        return (k + one_max);
    else
        return (n - one_max);
}

int eval_jump(bool *x, int n, int k) {
    int one_max = eval_one_max(x, n);
    return eval_jump_with_onemax(n, k, one_max);
}

int eval_onemax_from_jump(int n, int jump_size, int jump_value) {
    if (jump_value < jump_size)
        return n - jump_value;
    return jump_value - jump_size;
}


int diff, k, one_max;

int eval_jump_update(bool *x, int n, int jump_size, int *flippos, int fitness) {
    diff = k = 0;

    while (flippos[k] != -1) {
        if (x[flippos[k]] == 1) {
            diff -= 1;
        } else {
            diff += 1;
        }
        k++;
    }
    one_max = eval_onemax_from_jump(n, jump_size, fitness);
    return eval_jump_with_onemax(n, jump_size, diff + one_max);
}

int is_optimum_jump_function(int n, int jump, int fitness) {
    return (fitness == n + jump);
}