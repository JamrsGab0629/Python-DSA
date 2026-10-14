def bubble(arr,size):
    for i in range(size - 1):
        for j in range(size - i - 1):
            if arr[j] > arr[j + 1]:
                temp = arr[j]
                arr[j] = arr[j + 1]
                arr[j + 1] = temp



arr = [6,2,12,9]
size = len(arr)

bubble(arr,size)

for i in range(size):
    print(arr[i])