#include <iostream>
#include <omp.h>

int main(){
   double t0 = omp_get_wtime();
    int sum = 0;
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < 10000; i++) sum += i;


double t1 = omp_get_wtime();
std::cout << "elapsed = " << (t1 - t0) << " s\n";


   

    return 0;
}