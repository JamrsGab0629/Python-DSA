#include <iostream>
#include <string>
#include <algorithm>

bool anagrams(std::string str1,std::string str2){
    int size1 = (int)str1.length();
    int size2 = (int)str2.length();

    if (size1 != size2){
        return false;
    }

    std::sort(str1.begin(),str1.end());
    std::sort(str2.begin(),str2.end());

    for(int i = 0; i < size1 - 1; i++){
        if(str1[i] != str2[i]){
            return false;
        }
    }
    return true;
    

}

int main(){
    std::string str1 = "listen";
    std::string str2 = "silen";

    std::cout << std::boolalpha << anagrams(str1,str2); 
    return 0;
}