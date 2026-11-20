#include <iostream>
#include <algorithm>
#include <omp.h>

const int N = 4;          // philosophers = forks
const int MEALS = 3;

int main() {
    double time1 = omp_get_wtime(); 

    omp_lock_t fork[N];
    for (int i = 0; i < N; i++) omp_init_lock(&fork[i]);

    #pragma omp parallel num_threads(N)
    {
        int me = omp_get_thread_num();
        int left = me;
        int right = (me + 1) % N;

        int first = std::min(left, right);    // lower fork first
        int second = std::max(left, right);

        for (int meal = 1; meal <= MEALS; meal++) {
            omp_set_lock(&fork[first]);
            omp_set_lock(&fork[second]);

            #pragma omp critical
            std::cout << "philosopher " << me << " eats meal " << meal << "\n";

            omp_unset_lock(&fork[second]);
            omp_unset_lock(&fork[first]);
        }
    }
    double time2 = omp_get_wtime();
    for (int i = 0; i < N; i++) omp_destroy_lock(&fork[i]);

    std::cout << "The time is " << time2 - time1;
    return 0;
}