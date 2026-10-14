class Node:
    def __init__(self,val):
        self.val = val
        self.next = None


class LinkedList:
    def __init__(self):
        self.head = None


    def createnode(self,val):
        newnode = Node(val)

        if self.head is None:
            self.head = newnode
            return 

        temp = self.head
        while temp.next:
            temp = temp.next

        temp.next = newnode 

    def printlist(self):
        temp = self.head
        while temp:
            print(temp.val,end="->")
            temp = temp.next
            
    def detectCycle(self):
        fast = self.head 
        slow = self.head

        while fast and fast.next:
            slow = slow.next
            fast = fast.next.next

            if slow is fast:
                return True
        return False

li = LinkedList()

li.createnode(12)

li.createnode(12)

li.createnode(13)

print(li.detectCycle())
li.printlist()