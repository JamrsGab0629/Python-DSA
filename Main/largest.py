# to find the largest we need to search through the array to find the bggest number every time we found it it will go to largest variable

def findLargest(arr):
     largest = arr[0]
     for i in arr:
          if i > largest:
               largest = i
     return largest


    
     
arr = [1,8,9,2,3]

result = findLargest(arr)

print(result)
  