class Node:
    def __init__(self,val):
        self.val = val
        self.next = None


class linkedList:
    def __init__(self):
         self.head = None

    def insertatbeginning(self,val):
        new = Node(val)
        new.next = self.head
        self.head = new



    def printlist(self):
        temp = self.head
        while temp:
            print(temp.val)
            temp = temp.next

    def insertatend(self,val):
        new = Node(val)
        
        if self.head is None:
            self.insertatbeginning(val)
            return

        temp = self.head
        while temp.next != None:
            temp = temp.next

        temp.next = new

    def insertatpos(self,val,pos):
        new = Node(val)

        if pos < 0:
            raise IndexError("position less than zero was not allowed")

        if pos == 0:
            self.insertatbeginning(val)
            return

        temp = self.head
        for i in range(pos - 1):
            temp = temp.next


        if temp is None:
            raise IndexError("temp is none")
        
        new.next = temp.next
        temp.next = new

    def deletefirst(self):
        if self.head is None:
            raise IndexError("No element on the list please insert " )

        self.head = self.head.next

        


li = linkedList()

li.insertatbeginning(10)

li.insertatend(100)
li.insertatpos(30,1)
li.printlist()

