def getSquare(arr):
    left = 0
    n = len(arr)
    right = n - 1
    result = [0] * n
    pos = n - 1
    while left <= right:
        if abs(arr[left]) > abs(arr[right]):
             result[pos] = arr[left] * arr[left]
             left += 1
        else:
            result[pos] = arr[right] * arr[right]
            right -= 1
        pos -= 1
    return result

     
