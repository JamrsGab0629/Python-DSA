#include <iostream>
#include <random>
#include <vector>
#include <omp.h>
#include <algorithm>

void print_hist(const std::vector<long long>& hist, const char* label) {
    long long total = 0;

    #pragma omp parallel for reduction(+:total)
    for (int b = 0; b < (int)hist.size(); ++b) {
        std::cout << "bucket " << b << ": " << hist[b] << "\n";
        total += hist[b];
    }
    std::cout << label << " total = " << total << "\n";   // must be 100000000
}

int main() {
    const int n = 100'000'000;
    const int B = 10;                       // number of buckets

    std::vector<int> data(n);
    #pragma omp parallel
     {
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0, 99);
    #pragma omp for 
    for (int i = 0; i < n; ++i)
        data[i] = dist(gen);                // serial fill
    }

    // Serial histogram
    std::vector<long long> hist(B, 0);
    double t0 = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < n; ++i)
        #pragma omp atomic
        hist[data[i] / 10]++;
    double t1 = omp_get_wtime();

    std::cout << "atomic: " << (t1 - t0) << " s\n";
        print_hist(hist, "atomic");    
        std::fill(hist.begin(), hist.end(), 0);

    #pragma omp parallel 
    {
    #pragma omp for
    for (int b = 0; b < B; ++b)
        std::cout << "bucket " << b << ": " << hist[b] << "\n";
}
 
}