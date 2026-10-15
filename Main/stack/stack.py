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

    def printstack(self):
        temp = self.top

        while temp != None:
            print(temp.val)
            temp = temp.next

    def pop(self):
        if self.top is None:
            return

        temp = self.top
        self.top = self.top.next
        return temp.val

    def peep(self):
        if self.top is None:
            return

        return self.top.val

   


stack = Stack()

stack.push(90)
stack.push(11)
print(stack.peep())
