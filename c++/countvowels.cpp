#include <iostream>
#include <string>

int countvowel(char vowel[],std::string test,int size){
    int count = 0;
    for(char n : test){
        char low = std::tolower(n);
        for(int i = 0; i < size; i++){
            if(low == vowel[i]){
                count++;
            }
        }
    }
    return count;

}
int main(){
    char vowel[] = {'a','e','i','o','u'};
    int size = sizeof(vowel) / sizeof(vowel[0]);
    std::string test = "aelo";
    int result = countvowel(vowel,test,size);
    std::cout << result;
    return 0;
}