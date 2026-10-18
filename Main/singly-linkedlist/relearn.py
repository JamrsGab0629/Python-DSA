class Node:
    def __init__(self,val):
        self.val = val
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None


    def insertatbeggining(self,val):
        newnode = Node(val)

        newnode.next = self.head
        self.head = newnode 


    def printlist(self):
        temp = self.head
        while temp:
            print(temp.val)
            temp = temp.next
    def insertatend(self,val):
        newnode = Node(val)

        if self.head is None:
            self.head = newnode
            return 

        temp = self.head
        while temp.next:
            temp = temp.next

        temp.next = newnode

    def deletehead(self):
        if self.head is None:
            return

        temp = self.head.next
        self.head = self.head.next
        return temp.val
    
    def deletelast(self):
        if self.head is None:
            return
        temp = self.head
        count = 0

        if self.head.next is None:
            self.head = None
            return
        
        while temp:
            count += 1
            temp = temp.next

        curr = self.head
        for i in range(count - 2):
            curr = curr.next

        curr.next = curr.next.next

    def reversed(self):
        if self.head is None:
            return 

        curr = self.head
        prev = None

        while curr:
            nxt = curr.next
            curr.next = prev
            prev = curr
            curr = nxt
        self.head = prev

        

li = LinkedList()
li.insertatend(11)
li.insertatend(10)
li.insertatend(90)
li.deletelast()
li.reversed()
li.printlist()
