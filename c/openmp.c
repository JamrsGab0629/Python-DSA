// OpenMP benchmark: runs the chosen scenario 10 times and reports
// real, user and sys time for each run, plus the averages.
// Compile: gcc -O0 -fopenmp bench.c -o bench
// Usage:   ./bench <scenario 1|2|3> <threads> [iterations] [runs]
//   1 = plain loop, 2 = loop with counter, 3 = loop with print
// Examples: ./bench 1 1
//           ./bench 2 4
//           ./bench 3 1 1000000 > /dev/null   (print test: use smaller N!)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <omp.h>

// wall-clock ("real") seconds using time.h
static double real_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// cumulative user and sys CPU seconds of this process (all threads added up)
static void cpu_sec(double *user, double *sys) {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    *user = ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1e6;
    *sys  = ru.ru_stime.tv_sec + ru.ru_stime.tv_usec / 1e6;
}

int main(int argc, char **argv) {
    int scenario  = argc > 1 ? atoi(argv[1]) : 1;
    int threads   = argc > 2 ? atoi(argv[2]) : 1;
    long long n   = argc > 3 ? atoll(argv[3]) : 1000000000LL;
    int runs      = argc > 4 ? atoi(argv[4]) : 10;
    long long counter = 0;
    double sum_real = 0, sum_user = 0, sum_sys = 0;

    omp_set_num_threads(threads);

    for (int r = 1; r <= runs; r++) {
        counter = 0;
        double u0, s0, u1, s1;
        cpu_sec(&u0, &s0);
        double t0 = real_sec();

        if (scenario == 1) {
            #pragma omp parallel for
            for (long long i = 0; i < n; i++) {
                __asm__ volatile("");   // keeps the compiler from deleting the empty loop
            }
        } else if (scenario == 2) {
            #pragma omp parallel for reduction(+:counter)
            for (long long i = 0; i < n; i++) {
                counter++;
            }
        } else {
            #pragma omp parallel for
            for (long long i = 0; i < n; i++) {
                printf("%lld\n", i);
            }
        }

        double real = real_sec() - t0;
        cpu_sec(&u1, &s1);
        double user = u1 - u0, sys = s1 - s0;

        sum_real += real; sum_user += user; sum_sys += sys;
        fprintf(stderr, "Run %2d: scenario=%d threads=%d n=%lld counter=%lld | real=%.3f s user=%.3f s sys=%.3f s\n",
                r, scenario, threads, n, counter, real, user, sys);
    }

    fprintf(stderr, "Average over %d runs: real=%.3f s user=%.3f s sys=%.3f s\n",
            runs, sum_real / runs, sum_user / runs, sum_sys / runs);
    return 0;
}