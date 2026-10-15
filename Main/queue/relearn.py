class Node:
    def __init__(self,val):
        self.val = val
        self.next = None

class Queue:
    def __init__(self):
        self.head = None
        self.tail = None

    def enqueue(self,val):
        newnode = Node(val)

        if self.head is None:
            self.head = newnode
            return
        

        temp = self.head
        while temp.next:
            temp = temp.next

        temp.next = newnode 

    def enqueueTail(self,val):
        newnode = Node(val)
        if self.head is None:
           self.head = newnode 
           self.tail = newnode
        else:
            self.tail.next = newnode
            self.tail = newnode 
            
    def dequeue(self):
        if self.head is None:
            return

        temp = self.head.next
        self.head = self.head.next

        return temp.val


    def peek(self):
        if self.head is None:
            return 

        return self.head.val
        


queue = Queue()

queue.enqueueTail(90)
queue.enqueueTail(22)
queue.dequeue()
print(queue.peek())
        