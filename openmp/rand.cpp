#include <iostream>
#include <random>
#include <omp.h>

int main() {
    const int n = 100'000'000;
    std::vector<int> v(n);
    
    // rand()
    std::srand(42);
    double t0 = omp_get_wtime();
    for (int i = 0; i < n; ++i)
        v[i] = 1 + std::rand() % 100;
    double t1 = omp_get_wtime();

    // mt19937
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, 100);
    double t2 = omp_get_wtime();
    for (int i = 0; i < n; ++i)
        v[i] = dist(gen);
    double t3 = omp_get_wtime();

    std::cout << "rand():  " << (t1 - t0) << " s\n";
    std::cout << "mt19937: " << (t3 - t2) << " s\n";
}