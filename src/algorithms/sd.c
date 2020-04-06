#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "../fitness/fitness.h"
#include "mutation.h"
#include "utils.h"
#include "../fitness/need_high_mut.h"

char charray[2000];
bool x[10000];
int flip_pos[10000];

bool sd(int n, long long max_iter, int jump, pcg32_random_t *mypcg_state) {
    bool new_change;
    int i, k;
    long long counter = 0, iter;
    int fitness, best_fit;

    double current_rate = 1;

    for (i = 0; i < n; ++i) {
        x[i] = ((pcg32_random_r(mypcg_state) <= UINT32_MAX / 2.0));
    }

    best_fit = eval_jump(x, n, jump);
    double counter_limit = counter_limit_cal(n, current_rate);

    for (iter = 0; iter < max_iter; ++iter) {

        if (counter > counter_limit) {
            current_rate = current_rate + 1.;
            counter_limit = counter_limit_cal(n, current_rate);
            counter = 0;
            printf("New rate is %0.1lf, and check it for %lf times.\n", current_rate, counter_limit);
        }

        new_change = flip_uniformly(n, (double) (current_rate) / (double) n, mypcg_state, flip_pos);

        if (!new_change) {
            counter++;
            continue;
        }

        fitness = eval_jump_update(x, n, jump, flip_pos, best_fit);

        if (fitness > best_fit) {
            best_fit = fitness;
            flip_bits(x, flip_pos);
            printf("New best fitness: %d at step: %lld\n", fitness, iter);

            counter = 0;
            current_rate = 1.;
            counter_limit = counter_limit_cal(n, current_rate);
            printf("New rate reset %lf\n", current_rate);

            if (is_optimum_jump_function(n, jump, best_fit)) {
                printf("Opt reached in %lld iterations.\n", iter);
                sprintf(charray, "%lld ", iter);
                return true;
            }

        } else if (fitness == best_fit && current_rate == 1) {
            flip_bits(x, flip_pos);
            ++counter;
        } else {
            ++counter;
        }
    }
    return false;
}


bool sd_nhm(int n, long long max_iter, int xi, pcg32_random_t *mypcg_state) {
    bool new_change;
    int i, k;
    long long counter = 0, iter;
    int fitness, best_fit;

    double current_rate = 1;

    int block_size = ceilf(powf(n, 1.0 / 4.0));
    int block_number = ceilf(2.0 * powf(n, 0.5) / 3.0 * xi);
    int m = block_number * block_size;

    for (i = 0; i < n; ++i) {
        x[i] = ((pcg32_random_r(mypcg_state) <= UINT32_MAX / 2.0));
    }

    best_fit = need_high_mut(x, n, m, block_size);
    double counter_limit = counter_limit_cal(n, current_rate);

    for (iter = 0; iter < max_iter; ++iter) {

        if (counter > counter_limit) {
            current_rate = current_rate + 1.;
            counter_limit = counter_limit_cal(n, current_rate);
            counter = 0;
            printf("New rate is %0.1lf, and check it for %lf times.\n", current_rate, counter_limit);
        }

        new_change = flip_uniformly(n, (double) (current_rate) / (double) n, mypcg_state, flip_pos);

        if (!new_change) {
            counter++;
            continue;
        }

        fitness = need_high_mut_update(x, n, m, block_size, flip_pos);

        if (fitness > best_fit) {
            best_fit = fitness;
            flip_bits(x, flip_pos);
            printf("New best fitness: %d at step: %lld\n", fitness, iter);

            counter = 0;
            current_rate = 1.;
            counter_limit = counter_limit_cal(n, current_rate);

            if (is_optimum_need_high_mut(n, m, block_number, block_size, best_fit)) {
                printf("Opt reached in %lld iterations.\n", iter);
                sprintf(charray, "%lld global ", iter);
                return true;
            }
            if (is_inlocal_optimum_need_high_mut(n, m, block_number, block_size, best_fit)) {
                printf("Opt reached in *local optimum* at %lld iterations.\n", iter);
                sprintf(charray, "%lld local ", iter);
                return true;
            }

        } else if (fitness == best_fit && current_rate == 1) {
            flip_bits(x, flip_pos);
            ++counter;
        } else {
            ++counter;
        }
    }
    return false;
}
