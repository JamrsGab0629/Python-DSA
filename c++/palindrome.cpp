
#include <iostream>
#include <string>
#include <cctype>


bool is_palindrome(std::string word){
    int left = 0;
    int right =  word.length() - 1;
    while(left < right){
        if (std::lower(word[left]) != std::lower(word[right])){
            return false;
        }
        left++;
        right--;

    }
    return true;

}

int main(){

    std::string word = "racecar";
    
    std::cout << std::boolalpha << is_palindrome(word) << "\n";


}