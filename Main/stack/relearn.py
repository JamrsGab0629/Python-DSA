class Node:
    def __init__(self,val):
        self.val = val
        self.next = None

class Stack:
    def __init__(self):
        self.top = None


    def push(self,val):
        newnode = Node(val)

        newnode.next = self.top
        self.top = newnode

    def pop(self):
        if self.top is None:
            return 

        temp = self.top.next
        self.top = self.top.next

        return temp.val

    def peek(self):
        if self.top is None:
            return

        return self.top.val
    
    def printstack(self):
        temp = self.top
        while temp:
            print(temp.val)
            temp = temp.next

stack = Stack()

stack.push(10)
stack.push(11)




stack.printstack()