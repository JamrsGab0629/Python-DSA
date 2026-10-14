def bubble(arr,size):
    for i in range(size - 1):
        for j in range(size - i - 1):
            if arr[j] < arr[j + 1]:
                temp = arr[j]
                arr[j] = arr[j + 1]
                arr[j + 1] = temp



arr = [1,2,3,4,5,6,7]
size = len(arr)

bubble(arr,size)

for i in range(size):
    print(arr[i])

