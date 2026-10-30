#include <iostream>
#include <string>

int main(){
    int arr[] = {1,2,3,4,5,6};
    int val = 12;
    int* p = &val;
    
    std::cout << *p << "\n";
    std::cout << p << "\n";
    std::cout << *arr << "\n";
    std::cout << arr;
    
    
    return 0;
}