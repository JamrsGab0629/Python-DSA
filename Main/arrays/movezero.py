def movezero(arr):
    write = 0
    for read in range(len(arr)):
        if arr[read] != 0:
            arr[write],arr[read] = arr[read],arr[write]
            write += 1

arr = [0,0,1,2,3,4,5]

movezero(arr)
for i in range(len(arr)):
    print(arr[i])

        