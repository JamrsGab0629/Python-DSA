def is_palidrome(inp):
    left = 0
    right = len(inp) - 1

    while left <= right:
        if inp[left] != inp[right]:
            return False
        left += 1
        right -= 1
    return True




print(is_palidrome("racecar"))