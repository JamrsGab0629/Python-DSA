#include <iostream>
#include <random>
#include <vector>
#include <omp.h>

std::vector<int> randomArr(int n , int low, int high){
    
    std::vector<int> v(n);
    #pragma omp parallel 
    {
    std::mt19937 gen(42 + omp_get_thread_num());
    std::uniform_int_distribution<int> dist(low,high);

    #pragma omp for
    for(int i = 0; i < n; ++i){
        v[i] = dist(gen);
    }
  
    }

 return v;
}
int main(){

int n = 100;

std::vector<int> v = randomArr(n,1,100);


long long sum = 0;
#pragma omp parallel for reduction(+:sum) 
for(int i = 0; i < n; i++){
    sum += v[i];
}
 


std::cout << sum;

}