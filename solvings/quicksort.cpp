#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <omp.h>

void swapval(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

std::vector<int> randomARR(int n, int low, int high) {
    std::vector<int> v(n);
    #pragma omp parallel
    {
        std::mt19937 gen(42 + omp_get_thread_num());
        std::uniform_int_distribution<int> dist(low, high);

        #pragma omp for
        for (int i = 0; i < n; i++) {
            v[i] = dist(gen);
        }
    }
    return v;
}

// splits v[left..right] into  < pivot | == pivot | > pivot
void partitionRange(std::vector<int>& v, int left, int right, int& lessEnd, int& greaterStart) {
    int pivot = v[left + (right - left) / 2];
    lessEnd = left;
    greaterStart = right;
    int i = left;

    while (i <= greaterStart) {
        if (v[i] < pivot) {
            swapval(v[i], v[lessEnd]);
            lessEnd++;
            i++;
        } else if (v[i] > pivot) {
            swapval(v[i], v[greaterStart]);
            greaterStart--;
        } else {
            i++;
        }
    }
}

void quicksortSerial(std::vector<int>& v, int left, int right) {
    if (left >= right) return;

    int lessEnd, greaterStart;
    partitionRange(v, left, right, lessEnd, greaterStart);

    quicksortSerial(v, left, lessEnd - 1);
    quicksortSerial(v, greaterStart + 1, right);
}

void quicksortParallel(std::vector<int>& v, int left, int right) {
    if (left >= right) return;

    if (right - left < 10000) {              // cutoff: small pieces run serially
        quicksortSerial(v, left, right);
        return;
    }

    int lessEnd, greaterStart;
    partitionRange(v, left, right, lessEnd, greaterStart);

#pragma omp task firstprivate(left, right, lessEnd, greaterStart)
quicksortParallel(v, left, lessEnd - 1);

#pragma omp task firstprivate(left, right, lessEnd, greaterStart)
quicksortParallel(v, greaterStart + 1, right);
}

int main() {
    int n = 10000000;                                  // start at 10 million
    std::vector<int> original = randomARR(n, 1, 10000);   // many duplicates: fine now
    std::vector<int> a = original;
    std::vector<int> b = original;

    double t0 = omp_get_wtime();
    quicksortSerial(a, 0, n - 1);
    double serialTime = omp_get_wtime() - t0;

    double t1 = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        quicksortParallel(b, 0, n - 1);
    }
    double parallelTime = omp_get_wtime() - t1;

    std::cout << "serial:   " << serialTime << " s, "
              << (std::is_sorted(a.begin(), a.end()) ? "sorted" : "NOT sorted") << "\n";
    std::cout << "parallel: " << parallelTime << " s, "
              << (std::is_sorted(b.begin(), b.end()) ? "sorted" : "NOT sorted") << "\n";
    std::cout << "speedup:  " << serialTime / parallelTime << "x\n";
    return 0;
}