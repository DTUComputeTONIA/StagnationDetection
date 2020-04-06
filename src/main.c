#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "algorithms/sd.h"
#include "algorithms/sasd.h"

int main(int argc, char *argv[]) {

    if (argc < 5) {
        printf("The number of arguments to run an experiment is not enough. Read the documentation.\n");
        exit(1);
    }

    int seed_number = time(NULL);
    pcg32_random_t mypcg_state = {seed_number, 6687694301};// 1786059013,6687694301}; // init PCG

    int n;
    int jump_size;
    int lambda;
    int xi;
    long long max_iter;
    if (strcmp(argv[1], "jump") == 0) {
        if (strcmp(argv[2], "sd") == 0) {
            n = atoi(argv[3]);
            jump_size = atoi(argv[4]);
            max_iter = atoll(argv[5]);
            printf("SD-(1+1)EA on Jump_%d with n=%d and maximum %lld iterations.\n", jump_size, n, max_iter);
            sd(n, max_iter, jump_size, &mypcg_state);
        } else if (strcmp(argv[2], "sasd") == 0) {
            n = atoi(argv[3]);
            jump_size = atoi(argv[4]);
            lambda = atoi(argv[5]);
            max_iter = atoll(argv[6]);
            printf("SASD-(1+%d)EA on Jump_%d with n=%d and maximum %lld iterations.\n", lambda, jump_size, n, max_iter);
            sasd(n, max_iter, jump_size, &mypcg_state, lambda);
        } else {
            printf("In the second argument, you should declare the algorithm you want to run on Jump function. Choose "
                   "sd for SD-(1+1)EA or sasd for SASD-(1+λ)EA.\n");
        }
    } else if (strcmp(argv[1], "nhm") == 0) {
        printf("The NeedHighMutation experiment\n");
        if (strcmp(argv[2], "sd") == 0) {
            n = atoi(argv[3]);
            xi = atoi(argv[4]);
            max_iter = atoll(argv[5]);
            printf("SD-(1+1)EA on NeedHighMut_%d with n=%d and maximum %lld iterations.\n", xi, n, max_iter);
            sd_nhm(n, max_iter, xi, &mypcg_state);
        } else if (strcmp(argv[2], "sasd") == 0) {
            n = atoi(argv[3]);
            xi = atoi(argv[4]);
            lambda = atoi(argv[5]);
            max_iter = atoll(argv[6]);
            printf("SASD-(1+%d)EA on NeedHighMut_%d with n=%d and maximum %lld iterations.\n", lambda, xi, n, max_iter);
            sasd_nhm(n, max_iter, xi, &mypcg_state, lambda);
        } else {
            printf("In the second argument, you should declare the algorithm you want to run on Jump function. Choose "
                   "sd for SD-(1+1)EA or sasd for SASD-(1+λ)EA.\n");
        }
    } else {
        printf("In the first argument, you should declare the experiment you want to run. Choose jump for Jump or nhm "
               "for NeedHighMutation.\n");
    }
    return 0;
}