def binarysearch(arr,target):
    left = 0
    right = len(arr) - 1

   
    while left <= right:
       mid = (right + left) // 2
       if arr[mid] == target:
          return mid
       elif arr[mid] < target:
          left = mid + 1
       else:
          right = mid - 1

    return -1


arr = [1,2,3,4,5,6,7,8]
target = 1

result = binarysearch(arr,target)

print(f"the target is at index {result}")

