#include <iostream>
#include <vector>
#include <random>
#include <omp.h>

int main(){
int n = 100'000'000; 
std::vector<int> v(n);    
  
    #pragma omp parallel 
    {
  std::mt19937 gen(42);
  std::uniform_int_distribution<int> dist(1, 10);    

    #pragma omp for
    for (int i = 0; i < n; ++i)  // fill: your job is the loop body
        v[i] = dist(gen);
    }
    long long sum_s = 0;
    double t0 = omp_get_wtime();
    for (int i = 0; i < n; ++i)
    sum_s += v[i];
   double t1 = omp_get_wtime();
   
    long long  sum = 0;
    double time1 = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < n; i++){
        sum += v[i];

    }
    double time2 = omp_get_wtime();
    std::cout << "The total time of first is " << t1 - t0 << "\n";
    std::cout << "The total time of second is " << time2 - time1 << "\n";
    std::cout << sum;


return 0;
}
