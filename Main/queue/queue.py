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

        if self.tail is None:
            self.head = newnode
            self.tail = newnode
        else:
             self.tail.next = newnode
             self.tail = newnode

    def printqueue(self):
        temp = self.head
        while temp:
            print(temp.val)
            temp = temp.next
         
    def dequeue(self):
        if self.head is None:
            return
        temp = self.head
        self.head = self.head.next
        
        if self.head is None:
            self.tail = None

        return temp.val 

    def peek(self):
        if self.head is None:
            return 
        return self.head.val
    
    def is_empty(self):
        return self.head is None

queue = Queue()

queue.enqueue(10)
queue.enqueue(90)
queue.dequeue()
queue.printqueue()