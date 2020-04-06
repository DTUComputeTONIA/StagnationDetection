#ifndef EA_UTILS_H
#define EA_UTILS_H

#include <stdbool.h>
#include <stdint.h>

void flip_bits(bool *x, int *flip_pos);

int **inter_sat(char *file_name);

#endif //EA_UTILS_H
