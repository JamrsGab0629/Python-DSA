#include <iostream>
#include <string>

bool findrep(std::string str1){
    int counts[26] = {0};

for (char c : str1) {
    counts[c - 'a']++;
}
for (char c : str1){
    if(counts[c - 'a'] == 1){
        return true;
    }
}
return false;
}
int main(){
std::cout << std::boolalpha <<  findrep("aabbcd") << "\n";     // c
    std::cout << std::boolalpha << findrep("leetcode") << "\n";   // l
    std::cout << std::boolalpha << findrep("aabb") << "\n";       // -
    return 0;
}


