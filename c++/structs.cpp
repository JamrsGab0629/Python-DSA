#include <iostream>
#include <string>

struct  Studs
{
    std::string name;
    int age = 0;

    Studs(std::string name,int age){
        this->name  = name;
        this->age = age;
    }
    const std::string& getName(){
        return this->name;

    }
     int getAge() const {
        return this->age;
    }
     void printValue() const {
        std::cout << "The name is " << this->name << " And the age is " << this->age;
    }
};




int main(){
    std::string name;
    int age = 0;
    std::cout << "enter name";
    std::cin >> name;
    std::cout << "Enter age";
    std::cin >> age;
    Studs s(name,age);
    s.printValue();
    
    return 0;
}