#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

std::vector<int> randomARR(int n, int low, int high) {
    std::vector<int> v(n);
    #pragma omp parallel
    {
        std::mt19937 gen(42 + omp_get_thread_num());
        std::uniform_int_distribution<int> dist(low, high);

        #pragma omp for
        for (int i = 0; i < n; i++)
            v[i] = dist(gen);
    }
    return v;
}

void report(const char* name, double t_serial, double t_par, int threads) {
    double S = t_serial / t_par;
    double E = S / threads;
    std::cout << name << " | " << threads << " threads: serial=" << t_serial
              << "  parallel=" << t_par
              << "  speedup=" << S
              << "  efficiency=" << E * 100 << "%\n";
}

// ---------- parallel versions ----------
int findLargest(const std::vector<int>& arr, int n) {
    int result = arr[0];
    #pragma omp parallel for reduction(max:result)
    for (int i = 0; i < n; i++)
        if (arr[i] > result) result = arr[i];
    return result;
}

int findSmallest(const std::vector<int>& arr, int n) {
    int result = arr[0];
    #pragma omp parallel for reduction(min:result)
    for (int i = 0; i < n; i++)
        if (arr[i] < result) result = arr[i];
    return result;
}

long long totalSum(const std::vector<int>& arr, int n) {
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
        sum += arr[i];
    return sum;
}

// ---------- serial versions (baseline + answer key) ----------
int findLargestSerial(const std::vector<int>& arr, int n) {
    int result = arr[0];
    for (int i = 0; i < n; i++)
        if (arr[i] > result) result = arr[i];
    return result;
}

int findSmallestSerial(const std::vector<int>& arr, int n) {
    int result = arr[0];
    for (int i = 0; i < n; i++)
        if (arr[i] < result) result = arr[i];
    return result;
}

long long totalSumSerial(const std::vector<int>& arr, int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++)
        sum += arr[i];
    return sum;
}

int main() {
    const int n = 100'000'000;
    std::vector<int> v = randomARR(n, 1, 1000000);
    int threads = omp_get_max_threads();

    double t0, t1, s0, s1;

    // ---- sum ----
    s0 = omp_get_wtime();
    long long sumS = totalSumSerial(v, n);
    s1 = omp_get_wtime();
    t0 = omp_get_wtime();
    long long sumP = totalSum(v, n);
    t1 = omp_get_wtime();
    report("sum", s1 - s0, t1 - t0, threads);
    std::cout << "  " << (sumS == sumP ? "match" : "MISMATCH") << " (" << sumP << ")\n";

    // ---- max ----
    s0 = omp_get_wtime();
    int maxS = findLargestSerial(v, n);
    s1 = omp_get_wtime();
    t0 = omp_get_wtime();
    int maxP = findLargest(v, n);
    t1 = omp_get_wtime();
    report("max", s1 - s0, t1 - t0, threads);
    std::cout << "  " << (maxS == maxP ? "match" : "MISMATCH") << " (" << maxP << ")\n";

    // ---- min ----
    s0 = omp_get_wtime();
    int minS = findSmallestSerial(v, n);
    s1 = omp_get_wtime();
    t0 = omp_get_wtime();
    int minP = findSmallest(v, n);
    t1 = omp_get_wtime();
    report("min", s1 - s0, t1 - t0, threads);
    std::cout << "  " << (minS == minP ? "match" : "MISMATCH") << " (" << minP << ")\n";

    return 0;
}