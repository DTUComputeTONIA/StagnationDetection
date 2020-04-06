#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "../fitness/fitness.h"
#include "../fitness/need_high_mut.h"
#include "mutation.h"
#include "utils.h"


char charray[2000];
bool x[10000];
int flip_pos[10000];
int flip_pos_final[10000];

bool sasd(int n, long long max_iter, int jump, pcg32_random_t *mypcg_state, int lambda) {
    int new_change;
    int i, k;
    long long counter = 0, iter;
    int fitness, best_fit, local_fitness;

    double current_rate = 2, next_rate, local_rate, fitness_rate;

    for (i = 0; i < n; ++i) {
        x[i] = ((pcg32_random_r(mypcg_state) <= UINT32_MAX / 2));
    }

    best_fit = eval_jump(x, n, jump);
    double counter_limit = counter_limit_cal(n, current_rate);

    bool ambush = false;
    for (iter = 0; iter < max_iter; ++iter) {

        fitness = -1;
        fitness_rate = current_rate;

        if (ambush) {
            local_rate = current_rate;
            for (i = 0; i < lambda; i++) {
                local_rate = current_rate;
                new_change = flip_uniformly(n, (double) (local_rate) / (double) n, mypcg_state, flip_pos);
                if (!new_change)
                    continue;
                local_fitness = eval_jump_update(x, n, jump, flip_pos, best_fit);
                if (local_fitness >= fitness) {
                    fitness = local_fitness;
                    fitness_rate = local_rate;
                    k = 0;
                    while (flip_pos[k] != -1) {
                        flip_pos_final[k] = flip_pos[k];
                        k++;
                    }
                    flip_pos_final[k] = -1;
                }
            }


            if (fitness > best_fit) {
                best_fit = fitness;
                flip_bits(x, flip_pos_final);
                printf("New best fitness: %d at step: %lld with rate %lf\n", fitness, iter, fitness_rate);
                counter = 0;
                counter_limit = counter_limit_cal(n, current_rate);
                ambush = false;

                if (is_optimum_jump_function(n, jump, best_fit)) {
                    printf("Opt reached in %lld iterations.\n", iter);
                    sprintf(charray, "%lld ", iter * lambda);
                    return true;
                }

            } else {
                counter = counter + 1;
                if (counter > counter_limit) {
                    current_rate = fmin(current_rate + 1., n / 2);
                    counter_limit = counter_limit_cal(n, current_rate);
                    counter = 0;
                    printf("New rate is %0.1lf, and check it for %lf times.\n", current_rate, counter_limit);
                }
            }
        } else {
            for (i = 0; i < lambda; i++) {
                if (i < lambda / 2)
                    local_rate = current_rate / 2;
                else
                    local_rate = current_rate * 2;

                new_change = flip_uniformly(n, (double) (local_rate) / (double) n, mypcg_state, flip_pos);
                if (!new_change)
                    continue;
                local_fitness = eval_jump_update(x, n, jump, flip_pos, best_fit);
                if (local_fitness >= fitness) {
                    fitness = local_fitness;
                    fitness_rate = local_rate;
                    k = 0;
                    while (flip_pos[k] != -1) {
                        flip_pos_final[k] = flip_pos[k];
                        k++;
                    }
                    flip_pos_final[k] = -1;
                }
            }

            if (fitness >= best_fit) {
                if (fitness > best_fit) {
                    printf("New best fitness: %d at step: %lld with rate %lf\n", fitness, iter, fitness_rate);
                    counter = 0;
                    counter_limit = counter_limit_cal(n, current_rate);
                }
                best_fit = fitness;
                flip_bits(x, flip_pos_final);

                if (is_optimum_jump_function(n, jump, best_fit)) {
                    printf("Opt reached in %lld iterations.\n", iter);
                    sprintf(charray, "%lld ", iter * lambda);
                    return true;
                }


            }

            next_rate = assign_random(fitness_rate, assign_random(current_rate / 2, current_rate * 2, mypcg_state),
                                      mypcg_state);
            current_rate = fmin(fmax(2, next_rate), (double) n / 4);

            counter = counter + 1;

            if (counter > counter_limit) {
                ambush = true;
                current_rate = 2;
                counter_limit = counter_limit_cal(n, current_rate);
                counter = 0;
                printf("New rate is %0.1lf, and check it for %lf times.\n", current_rate, counter_limit);
            }

        }
    }
    return false;
}

bool sasd_nhm(int n, long long max_iter, int xi, pcg32_random_t *mypcg_state, int lambda) {
    int new_change;
    int i, k;
    long long counter = 0, iter;
    int fitness, best_fit, local_fitness;

    double current_rate = 2, next_rate, local_rate, fitness_rate;

    int block_size = ceilf(powf(n, 1.0 / 4.0));
    int block_number = ceilf(2.0 * powf(n, 0.5) / 3.0 * xi);
    int m = block_number * block_size;

    for (i = 0; i < n; ++i) {
        x[i] = ((pcg32_random_r(mypcg_state) <= UINT32_MAX / 2));
    }

    best_fit = need_high_mut(x, n, m, block_size);
    double counter_limit = counter_limit_cal(n, current_rate);
    double p;

    bool ambush = false;
    for (iter = 0; iter < max_iter; ++iter) {

        fitness = best_fit;
        fitness_rate = current_rate;

        if (ambush) {
            local_rate = current_rate;
            for (i = 0; i < lambda; i++) {
                local_rate = current_rate;
                new_change = flip_uniformly(n, (double) (local_rate) / (double) n, mypcg_state, flip_pos);
                if (!new_change)
                    continue;
                local_fitness = need_high_mut_update(x, n, m, block_size, flip_pos);

                if (local_fitness >= fitness) {
                    fitness = local_fitness;
                    fitness_rate = local_rate;
                    k = 0;
                    while (flip_pos[k] != -1) {
                        flip_pos_final[k] = flip_pos[k];
                        k++;
                    }
                    flip_pos_final[k] = -1;
                }
            }


            if (fitness > best_fit) {
                best_fit = fitness;
                flip_bits(x, flip_pos_final);
                printf("New best fitness: %d at step: %lld with rate %lf\n", fitness, iter, fitness_rate);
                counter = 0;
                counter_limit = counter_limit_cal(n, current_rate);
                ambush = false;

                if (is_optimum_need_high_mut(n, m, block_number, block_size, best_fit)) {
                    printf("Opt reached in %lld iterations.\n", iter);
                    sprintf(charray, "%lld global ", iter * lambda);
                    return true;
                }
                if (is_inlocal_optimum_need_high_mut(n, m, block_number, block_size, best_fit)) {
                    printf("Opt reached in *local optimum* at %lld iterations.\n", iter);
                    sprintf(charray, "%lld local ", iter * lambda);
                    return true;
                }

            } else {
                counter = counter + 1;
                if (counter > counter_limit) {
                    current_rate = fmin(current_rate + 1., n / 2);
                    counter_limit = counter_limit_cal(n, current_rate);
                    counter = 0;
                    printf("New rate is %0.1lf, and check it for %lf times.\n", current_rate, counter_limit);
                }
            }
        } else {
            for (i = 0; i < lambda; i++) {
                if (i < lambda / 2)
                    local_rate = current_rate / 2;
                else
                    local_rate = current_rate * 2;

                new_change = flip_uniformly(n, (double) (local_rate) / (double) n, mypcg_state, flip_pos);
                if (!new_change)
                    continue;
                local_fitness = need_high_mut_update(x, n, m, block_size, flip_pos);
                if (local_fitness >= fitness) {
                    fitness = local_fitness;
                    fitness_rate = local_rate;
                    k = 0;
                    while (flip_pos[k] != -1) {
                        flip_pos_final[k] = flip_pos[k];
                        k++;
                    }
                    flip_pos_final[k] = -1;
                }
            }

            if (fitness >= best_fit) {
                if (fitness > best_fit) {
                    counter = 0;
                    counter_limit = counter_limit_cal(n, current_rate);
                }
                best_fit = fitness;
                flip_bits(x, flip_pos_final);

                if (is_optimum_need_high_mut(n, m, block_number, block_size, best_fit)) {
                    printf("Opt reached in %lld iterations.\n", iter);
                    sprintf(charray, "%lld global ", iter * lambda);
                    return true;
                }
                if (is_inlocal_optimum_need_high_mut(n, m, block_number, block_size, best_fit)) {
                    printf("Opt reached in *local optimum* at %lld iterations.\n", iter);
                    sprintf(charray, "%lld local ", iter * lambda);
                    return true;
                }

            }

            next_rate = assign_random(fitness_rate, assign_random(current_rate / 2, current_rate * 2, mypcg_state),
                                      mypcg_state);
            current_rate = fmin(fmax(2, next_rate), (double) n / 4);

            counter = counter + 1;

            if (counter > counter_limit) {
                ambush = true;
                current_rate = 2;
                counter_limit = counter_limit_cal(n, current_rate);
                counter = 0;
                printf("New rate is %0.1lf, and check it for %lf times.\n", current_rate, counter_limit);
            }

        }
    }
    return false;
}
