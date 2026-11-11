#include <iostream>
#include <random>
#include <vector>
#include <omp.h>

int main() {
    const int n = 100'000'000;
    std::vector<int> arr(n);

    // Fill serially with a fixed seed (same numbers every run)
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    for (int i = 0; i < n; ++i)
        arr[i] = dist(gen);

    // Serial version
    int s_largest = arr[0], s_smallest = arr[0];
    double t0 = omp_get_wtime();
    for (int i = 0; i < n; ++i) {
        if (arr[i] > s_largest)  s_largest  = arr[i];
        if (arr[i] < s_smallest) s_smallest = arr[i];
    }
    double t1 = omp_get_wtime();

    // Parallel version
    int largest = arr[0], smallest = arr[0];
    double t2 = omp_get_wtime();
    #pragma omp parallel for reduction(max:largest) reduction(min:smallest)
    for (int i = 0; i < n; ++i) {
        if (arr[i] > largest)  largest  = arr[i];
        if (arr[i] < smallest) smallest = arr[i];
    }
    double t3 = omp_get_wtime();

    std::cout << "serial:   max = " << s_largest << ", min = " << s_smallest
              << ", time = " << (t1 - t0) << " s\n";
    std::cout << "parallel: max = " << largest << ", min = " << smallest
              << ", time = " << (t3 - t2) << " s\n";
    std::cout << "speedup = " << (t1 - t0) / (t3 - t2) << "x\n";
    return 0;
}