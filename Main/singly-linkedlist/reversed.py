class Node:
    def __init__(self,val):
        self.val = val
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None


    def insert(self,val):
        new = Node(val)

        if self.head is None:
            self.head = new
            return

        temp = self.head
        while temp.next != None:
            temp = temp.next

        temp.next = new

    def printlist(self):
        temp = self.head
        while temp != None:
            print(temp.val, end="->")
            temp = temp.next

    def reversedlist(self):
        curr = self.head
        prev = None
        while curr:
            nxt = curr.next
            curr.next = prev
            prev = curr
            curr = nxt
        self.head = prev


li = LinkedList()

li.insert(10)

li.insert(12)

li.insert(11)

li.printlist()

print("\n previous \n reversed")

li.reversedlist()
li.printlist()
        
