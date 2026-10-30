#include <iostream>
#include <string>
#include <stdexcept>

int main(){
    std::cout << "hello world" << "\n";
    
    int age;
    std::cout << "whats your age" << "\n"; 
    std::cin >> age;
    if (age < 0){
        throw std::runtime_error("negative age was not allowed ");
    }


    if (age > 18){
        std::cout << "you are an adult ";
    }
    else if (age > 60 ){
        std::cout << " you are an adult senior ";
    }

   
}