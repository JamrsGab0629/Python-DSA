def linearsearch(arr,size,target):
    for i in range(size):
        if arr[i] == target:
            return i
    return -1

arr = [1,2,3,4,5,6,7]
target = 10
size = len(arr)

result = linearsearch(arr,size,target)

print(f"the target is on index {result}")
