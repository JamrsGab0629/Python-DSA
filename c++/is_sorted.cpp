#include <iostream>

bool is_sorted(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    int a[] = {1, 2, 2, 5};
    int b[] = {3, 1, 2};
    std::cout << std::boolalpha;
    std::cout << is_sorted(a, 4) << "\n";   // true
    std::cout << is_sorted(b, 3) << "\n";   // false
    return 0;
}