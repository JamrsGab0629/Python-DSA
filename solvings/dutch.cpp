#include <iostream>

void swap(int &a,int &b){
    int temp = a;
    a = b;
    b = temp;

}



void dutch(int arr[],int left,int right,int &greater,int &lessthan){
    lessthan = left;
    greater = right;
    int pivot = arr[(right + left) / 2];
    int i = left;

    while(i <= greater){
        if(arr[i] < pivot){
            swap(arr[i],arr[lessthan]);
            lessthan++;
            i++;
        }
        else if (arr[i] > pivot){
            swap(arr[i],arr[greater]);
            greater--;
        }
        else{
            i++;
        }
    }


}

void quicksort(int arr[],int left,int right){
    if (left >= right) return;
    int greater,lessthan;
    dutch(arr,left,right,greater,lessthan);

    quicksort(arr,left,lessthan - 1);
    quicksort(arr,greater + 1,right);

}   
int main(){

    int arr[] = {2,1,4,5,6,7,9,0};
    int size = sizeof(arr)/sizeof(arr[0]);
    quicksort(arr,0,size - 1);

    for(int i = 0; i < size; i++){
        std::cout << arr[i];

    }

    return 0;
}