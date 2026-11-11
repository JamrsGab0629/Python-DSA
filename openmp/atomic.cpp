#include <iostream>
#include <omp.h>
#include <cmath>
double expensive(int i) {
    double x = 0;
    for (int k = 1; k < 100'000; ++k)
        x += std::sin(i * 0.001 + k);   // some slow, real work
    return x;
}

int main(){
    int counter = 0;
    #pragma omp parallel for
    for(int i = 0; i < 100; i++){
        double total = expensive(i);
        #pragma omp atomic
        counter++;

    }

     std::cout << counter << "\n";
    return 0;
}