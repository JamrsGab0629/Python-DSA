#include <iostream>

void swapelem(int arr[],int size){
    int temp = 0;
    int left = 0;
    int right = size - 1;
    while(left < right){
            temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;

        left++;
        right--;
    }
}
int main(){
    int arr[] = {-1,2,3,-4};
    int size = sizeof(arr) / sizeof(arr[0]);
    for(int i = 0; i < size; i++){
        std::cout << "before " << arr[i] << "\n";
    }

    swapelem(arr,size);

     for(int i = 0; i < size; i++){
        std::cout << "after " << arr[i] << "\n";
    }
}