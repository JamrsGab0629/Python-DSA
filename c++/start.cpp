#include <iostream>
#include <string>
int main(){
    std::cout << "hello world" << "\n";
    
    int age;
    std::cout << "whats your name" << "\n";
    std::cin >> age;

    if (age > 18){
        std::cout << "you are an adult ";
    }
    else if (age > 60 ){
        std::cout << " you are an adult senior ";
    }

    return 0;
}