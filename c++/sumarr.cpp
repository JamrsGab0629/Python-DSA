#include <iostream>
#include <exception>

int sumarr(int arr,int size){
   
    int total = 0;
    int left = 0;
    int right = size;
    int mid = (left + right) / 2;
    if (size <= 0 ){
        throw std::length_error("Theres no element on the array ");
    }
    for(int i = 0; i < mid - 1; i++){
        total += arr[left] + arr[right];
        left += 1;
        right -= 1;
    }
    return total;
}
int main(){

    int arr[] = {1,2,3,4,5};
     int size = sizeof(arr) / sizeof(arr[0]);
    int result = sumarr(arr);
    std::cout << result;

    return 0;
}