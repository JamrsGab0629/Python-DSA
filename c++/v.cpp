#include <iostream>
#include <vector>
#include <exception>

void rvect(std::vector<int> &v,int index){
    if(v.empty()){
        throw std::out_of_range("The vector is empty");
    }
    v.erase(v.begin() + index);
}
void insertatend(std::vector<int> &v,int val){
    v.push_back(val);
}
int main(){
    int index = 1;
    std::vector <int> v = {1,2};
    insertatend(v,11);
     rvect(v,index);
    for(int c : v){
        std::cout << c << ",";

    }

    return 0;
}