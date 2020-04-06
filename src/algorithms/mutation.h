#ifndef PCG32
#define PCG32

#include <stdint.h>

typedef struct {
    uint64_t state;
    uint64_t inc;
} pcg32_random_t;

#endif //PCG32

#ifndef EA_MUTATION_H
#define EA_MUTATION_H

#include <stdint.h>
#include <stdbool.h>

double counter_limit_cal(int n, double current_rate);

uint32_t pcg32_random_r(pcg32_random_t *rng);

int flip_uniformly(int, double, pcg32_random_t *, int *);

double assign_random(double a, double b, pcg32_random_t *rng);

#endif //EA_MUTATION_H



