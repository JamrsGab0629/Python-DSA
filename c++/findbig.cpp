#include <iostream>

int findbig(int arr[],int size){
    int largest = arr[0];
    for(int i = 1; i < size; i++){
        if (arr[i] > largest){
            largest = arr[i];

          
        }

    }
    return largest;
}


int findsmall(int arr[],int size){
    int smallest = arr[0];
    for(int i = 1; i < size; i++){
        if (arr[i] < smallest){
            smallest = arr[i];

          
        }

    }
    return smallest;
}


int main(){
    
int arr[] = {1,2,3,4,5,65,7};
int size = sizeof(arr) / sizeof(arr[0]);

int result1 = findbig(arr,size);
int result2 = findsmall(arr,size);
std::cout << "the largets is " <<  result1 << "amd the smallest is " << result2;

    return 0;
}