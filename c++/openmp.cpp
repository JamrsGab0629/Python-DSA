#include <iostream>
#include <omp.h>

int main() {
    int n = 100;
    #pragma omp parallel
    {
        // runs once, others wait here
        #pragma omp single
        {
            std::cout << "Setup by thread " << omp_get_thread_num() << "\n";
        }

        // all threads share the iterations
        #pragma omp for
        for (int i = 0; i < n; i++) {
             #pragma omp critical
            std::cout << i << " ";
        }

        #pragma omp single
        std::cout << "\nDone\n";
    }
    return 0;
}