#include <iostream>

int findlargest(int arr[],int size){
    int largest = arr[0];
    for(int i = 1; i < size; i++){
        if (arr[i] > largest){
            largest = arr[i];
        } 
        
    }
    return largest;

}
int findsecondbig(int arr[],int size){
    int second = arr[0];
    int largest = findlargest(arr,size);
    
    for(int i = 0; i < size; i++){
    if (arr[i] != largest){
        if (arr[i] > second && arr[i] < largest){
        second = arr[i];

    }
    
    }
       
    }
    return second;
}
int main(){
int arr[] = {1,2,3,4,5,6,6,7,8};
int size = sizeof(arr)/sizeof(arr[0]);

int largest = findlargest(arr,size);
int second = findsecondbig(arr,size);

std::cout << "the largest " << largest;
std::cout << " the second largest " << second;

}