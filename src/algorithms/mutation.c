#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "mutation.h"

static double geo_formula;
static double loguintdivided;
static double probability;

void update_parameters(double p) {
    geo_formula = log(1.0 - p);
    loguintdivided = log(UINT32_MAX - 1) / geo_formula;
}

uint32_t pcg32_random_r(pcg32_random_t *rng) {
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
    uint64_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint64_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

int i, j, k;

int flip_uniformly(int n, double p, pcg32_random_t *rng, int *flippos) {
    i = j = k = 0;

    if (fabs(probability - p) > 0.00001) {
        update_parameters(p);
        probability = p;
    }

    while ((j = (int) ((log((double) pcg32_random_r(rng)) / geo_formula - loguintdivided))) <=
           n - 1 - i) {
        i += j;
        flippos[k++] = i;
        ++i;
    }

    flippos[k] = -1;
    return k;
}

double counter_limit_cal(int n, double current_rate) {
    return pow(n * 2.71 / current_rate, current_rate) * 4 * log(n);
}

double assign_random(double a, double b, pcg32_random_t *rng) {
    if ((pcg32_random_r(rng) <= UINT32_MAX / 2.0)) {
        return a;
    }
    return b;
}