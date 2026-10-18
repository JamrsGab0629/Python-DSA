class Node:
    def __init__(self,val):
        self.val = val
        self.next = None
        self.prev = None

class Linkedlist:
    def __init__(self):
        self.head = None
        self.tail = None


    def insertatbeginning(self,val):
        newnode = Node(val)
        if self.head is None:
            self.head = newnode
            self.tail = newnode
            return 

        newnode.next = self.head
        newnode.head.prev = newnode
        self.head = newnode

    def insertatend(self,val):
        newnode = Node(val)
        if self.head is None:
            self.head = newnode 
            self.tail = newnode
            return 

        newnode.prev = self.tail 
        self.tail.next = newnode
        self.tail = newnode
            

            
        
    