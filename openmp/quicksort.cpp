#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

void swapval(int& a,int& b){
    int temp = a;
    a = b;
    b = temp;

}
std::vector<int> randomARR(int n,int low,int high){
    std::vector<int> v(n);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(low,high);

    for(int i = 0; i < n; i++){
        v[i] = dist(gen);
    }
    return v;

}
int partition(std::vector<int> &v,int left,int right){
    int pivot = v[right];
    int i = left - 1;
    for(int j = left; j < right; j++){
        if(v[j] <= pivot){
            i++;
            swapval(v[i],v[j]);
        }
    }
    swapval(v[i + 1],v[right]);
    return i + 1;
}

void quicksort(std::vector<int> &v,int left,int right){
    if(left < right){

        int pi = partition(v,left,right);

        quicksort(v,left,pi - 1);
        quicksort(v,pi + 1,right);
    }

}




int main(){
    int n = 1000000;

    std::vector <int> v(randomARR(n,1,1000000));
     quicksort(v,0,n - 1);
    for (int i = 0; i < n; i++){
        std::cout << v[i];
    }

std::cout << "\n";

    if (std::is_sorted(v.begin(), v.end())) {
        std::cout << "sorted\n";
    } else {
        std::cout << "NOT sorted\n";
    }
    return 0;
}