#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int k;

void flip_bits(bool *x, int *flip_pos) {
    k = 0;
    while (flip_pos[k] != -1) {
        x[flip_pos[k]] = !x[flip_pos[k]];
        ++k;
    }
}

