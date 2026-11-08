#include <omp.h>
#include <iostream>

int main(){
int count = 0; 
#pragma omp parallel for reduction(+:count)
for (int i = 0; i < 1000000; i++) {
    count++;            // no critical: race condition
}
std::cout << count;     // usually less than 1000000, and different every run

}