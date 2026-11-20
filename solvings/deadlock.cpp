#include <iostream>
#include <unistd.h>      // usleep
#include <omp.h>

const int N = 4;
const int MEALS = 3;

int main() {
    omp_lock_t fork[N];
    for (int i = 0; i < N; i++) omp_init_lock(&fork[i]);

    #pragma omp parallel num_threads(N)
    {
        int me = omp_get_thread_num();
        int left = me;
        int right = (me + 1) % N;

        for (int meal = 1; meal <= MEALS; meal++) {
            omp_set_lock(&fork[left]);            // everyone grabs LEFT first
            #pragma omp critical
            std::cout << "philosopher " << me << " took fork " << left
                      << ", wants fork " << right << std::endl;

            usleep(100000);                       // pause 0.1 s so the others grab their left fork too

            omp_set_lock(&fork[right]);           // waits forever: the neighbor holds it
            #pragma omp critical
            std::cout << "philosopher " << me << " eats meal " << meal << std::endl;

            omp_unset_lock(&fork[right]);
            omp_unset_lock(&fork[left]);
        }
    }
    return 0;
}