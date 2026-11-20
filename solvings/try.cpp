#include <iostream>
#include <omp.h>
#include <algorithm>

const int N = 4;
const int MEALS = 3; 

int main(){
    double time1 = omp_get_wtime();
    omp_lock_t fork[N];
    for(int i = 0; i < N; i++){
        omp_init_lock(&fork[i]);
    }
    #pragma omp parallel num_threads(N)
    {
        int me = omp_get_thread_num();
        int left = me;
        int right = (left + 1) % N;
        // int first = std::min(left,right);
        // int second = std::max(left,right);
        for(int meals = 0; meals <= MEALS; meals++){

            if (me % 2 == 0){
                 omp_set_lock(&fork[left]);
            omp_set_lock(&fork[right]);
            
        }else {
            omp_set_lock(&fork[right]);
            omp_set_lock(&fork[left]);
        }
            #pragma omp critical
            std::cout << "Philosopher " << me << "eats " << meals << "\n";

            omp_unset_lock(&fork[left]);
            omp_unset_lock(&fork[right]);

        }
    }
    double time2 = omp_get_wtime();
    for(int i = 0; i < N; i++) omp_destroy_lock(&fork[i]);
    
    std::cout << "The time is " << time2 - time1;

    return 0;
}