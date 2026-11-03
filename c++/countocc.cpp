#include <iostream>

int countocc(int arr[],int size,int target){
    int counter = 0;

    for(int i = 0; i < size; i++){
        if (arr[i] == target ){
            counter++;
        }
    }
    return counter;

}
int main(){
    int arr[] = {1,2,3,4,5,1,1};
    int target = 1;
    int size = sizeof(arr) /sizeof(arr[0]);

    int result = countocc(arr,size,target);
    std::cout << "The target appear " << result << "times";
    return 0;
}