 #include <iostream>
#include <unordered_map>
#include <string>
class kids{
    private:
        std::unordered_map<std::string ,int> map;
    public:
    void insertKid(){
        std::string name;
        int age = 0;
        std::cout << "enter your name ";
        std::cin >> name;
        std::cout << "enter your age ";
        std::cin >> age;
        map.insert({name ,age});
    }
    void removeKid(){
        if (map.size() == 0){
            return;            
        }
        std::string name;
        std::cout << "enter what name you wanna delete ";
        std::cin >> name;

        if(map.erase(name) == 0){
            std::cout << "not found";
            return;
        }
    }

    void showAll() const {
        for (const auto& [name, age] : map)
            std::cout << name << ": " << age << "\n";
    }

};
int main() {
    kids k;
    k.insertKid();
    
    k.showAll();
    return 0;
}       