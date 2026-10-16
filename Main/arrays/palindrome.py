def is_palindrome(inp):
    left = 0 
    right = len(inp) - 1

    while left < right:
        if inp[left] != inp[right]:
            return False
        left = left + 1
        right = right - 1

    return True

print(is_palindrome("racecar")) 
print(is_palindrome("abba"))     
print(is_palindrome("abca"))    
print(is_palindrome("abc"))     
print(is_palindrome(""))         
print(is_palindrome("a"))       