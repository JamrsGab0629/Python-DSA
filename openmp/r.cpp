#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

std::vector<int> randomARR(int n, int low,int high){
    std::vector<int> v(n);
    #pragma omp parallel
    {
        std::mt19937 gen(42 + omp_get_thread_num());
        std::uniform_int_distribution <int> dist(low,high);

        #pragma omp for
        for(int i = 0; i < n; i++){
            v[i] = dist(gen);
        }
    }
    return v;
}

int main(){
    int n = 10000000;
    std::vector <int> v = randomARR(n,1,1000000);
    long long sum = 0;
    int largest = v[0];
    double timesum1 = omp_get_wtime();
   
   

    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < n; i++){
        sum += v[i];
    }
    double timesum2 = omp_get_wtime();
    double timeMax1 = omp_get_wtime();
    #pragma omp parallel for reduction(max:largest)
    for(int i = 0; i < n; i++){
        if (v[i] > largest){
            largest = v[i];
        }
    }
   

 
 double timeMax2 = omp_get_wtime();
    std::cout << "The largest is " << largest << "\n";
    std::cout << sum << "\n";
    std::cout << "The time for sum is " << timesum2 - timesum1 << "\n";
    std::cout << "The time for max is " << timeMax2 - timeMax1;

    return 0;
}