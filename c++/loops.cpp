#include <iostream>
#include <algorithm>
#include <random>
void bubblesort(int arr[],int size){
  
    for(int i = 0; i < size - 1; i++){
        for (int j = 0; j < size - i - 1; j++){
            if (arr[j] > arr[j + 1]){
                std::swap(arr[j],arr[j+1]);
            }
        }

    }
}
int main(){

    int arr[] = {3,2,7,1,5};
    int size = sizeof(arr) / sizeof(arr[0]);
    bubblesort(arr,size);

    for (int i = 0; i < size; i++){
        std::cout << arr[i];
    }
    
}
