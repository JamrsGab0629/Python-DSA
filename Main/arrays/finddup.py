def finddupe(arr):
    seen = set()
    for i in arr:
        if i in seen:
            return True
        seen.add(i)
    return False


arr = [1,2,3,4,5,5]

result = finddupe(arr)

print(result)
