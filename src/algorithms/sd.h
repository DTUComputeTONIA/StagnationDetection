#ifndef PCG32
#define PCG32

#include <stdint.h>

typedef struct {
    uint64_t state;
    uint64_t inc;
} pcg32_random_t;

#endif //PCG32

#ifndef EA_EA_WITH_COUNTER_H
#define EA_EA_WITH_COUNTER_H

#include <stdbool.h>
#include <stdint.h>

bool sd(int n, long long max_iter, int jump, pcg32_random_t *mypcg_state);

bool sd_nhm(int n, long long max_iter, int xi, pcg32_random_t *mypcg_state);

#endif //EA_EA_WITH_COUNTER_H
