#include <iostream>
#include <omp.h>

int main(){

    int x = 10;
    // #pragma omp parallel for firstprivate(x) lastprivate(x) num_threads(4)
    // for(int i = 0; i < 100; i++){
    //     #pragma omp critical
    //     std::cout << "thread " << omp_get_thread_num() << " starts with x = " << x << "\n";
    //     x = i;
    // }
    // #pragma omp parallel for lastprivate(x) num_threads(4)
    // for(int i = 0; i < 100; i++){
    //     #pragma omp critical
    //     std::cout << "Threads " << omp_get_thread_num() << " stars with x = " << x << "\n";
    //     x = i;
    // }
    // #pragma omp parallel for private(x) num_threads(4)
    // for(int i = 0; i < 100; i++){
    //     #pragma omp critical
    //     std::cout << "Threads " << omp_get_thread_num() << " stars with x = " << x << "\n";
    //     x = i;
    // }
    #pragma omp parallel for firstprivate(x) lastprivate(x) num_threads(4)
    for (int i = 0; i < 100; i++){
        #pragma omp critical 
         std::cout << "Threads " << omp_get_thread_num() << " stars with x = " << x << "\n";
         x += i;
    }
    std::cout << "after loop " << x;

    return 0;
}