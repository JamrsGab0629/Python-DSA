#include <iostream>
#include <vector>


int main(){
    std::vector<int> v = {4,8,15,16,23,42};

    int total = 0;
    for(int i = 1; i < v.size();  i++){
        total += v[i];
    }
    std::cout << total;

}