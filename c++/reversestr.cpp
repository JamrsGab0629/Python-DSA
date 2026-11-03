#include <iostream>
#include <string>

void reversestr(std::string& word){
    int left = 0;
    int right = (int)word.length() - 1;

    while(left < right){
        char temp = word[left];
        word[left] = word[right];
        word[right] = temp; 
       

        left++;
        right--;
    }
    

}

int main(){
    std::string word = "HELLO";
    reversestr(word);

    std::cout << word;

    return 0;
}