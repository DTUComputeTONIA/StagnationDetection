#ifndef PCG32
#define PCG32
#include <stdint.h>
typedef struct {
    uint64_t state;
    uint64_t inc;
} pcg32_random_t;

#endif //PCG32

#ifndef EA_EA_WITH_COUNTER_ADJUSTABLE_H
#define EA_EA_WITH_COUNTER_ADJUSTABLE_H

#include <stdbool.h>
#include <stdint.h>

sasd(int n, long long max_iter, int jump, pcg32_random_t *mypcg_state, int);

sasd_nhm(int n, long long max_iter, int xi, pcg32_random_t *mypcg_state, int lambda);

#endif //EA_EA_WITH_COUNTER_ADJUSTABLE_H

