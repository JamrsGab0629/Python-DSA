#include <iostream>

void bubblesort(int arr[],int size){
    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size - i - 1; j++){
            if(arr[j] > arr[j + 1]){
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

bool twosum(int arr[],int size,int target){
    int left = 0;
    int right = size - 1;
    while(left < right){
        int sum = arr[left] + arr[right];
        if (sum == target){
            std::cout << "the indexes is at " << left << " and " << right << "\n";
            return true;
        }
        
        if (sum < target)
        {
            left += 1;
        }
        else {
            right -= 1;
        }
        
        
    }
    return false;
}

bool twosum2(int arr[],int size ,int target){
    for(int i = 0; i < size - 1; i++){
        for(int j = i + 1; j < size; j++){
            int sum = arr[i] + arr[j];
            if(sum == target){
                std::cout << "The indexes are in " << i << " and " << j << " and the value is " << arr[i] << " and " << arr[j] << "\n";
                return true;
            }
        }
    }
    return false;
}

int main(){

    int arr[] = {2,1,4,5,6};
    int target = 11;
    int size = sizeof(arr) / sizeof(arr[0]);
    std::cout << std::boolalpha << twosum2(arr,size,target);

    bubblesort(arr,size);

    std::cout << std::boolalpha << twosum(arr,size,target);
    return 0;
}