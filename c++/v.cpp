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
void printv(const std::vector <int> &v){
    if(v.empty()){
        throw std::out_of_range("the vector is empty");

    }
    for(int c : v){
        std::cout << c; 
    }
}
int main(){
    int index = 1;
    std::vector <int> v = {1,2};
    insertatend(v,11);
     rvect(v,index);
    printv(v);
    

    return 0;
}