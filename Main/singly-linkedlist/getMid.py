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
            print(temp.val)
            temp = temp.next

    def midNode(self):
        if self.head is None:
            return

        count = 0
        temp = self.head
        while temp != None:
            count += 1
            temp = temp.next

        mid = count // 2
        curr = self.head
        for i in range(mid):
            curr = curr.next

        return curr.val
            
    def deleteLast(self):
        if self.head is None:
            return
        
        if self.head.next is None:
            self.head = None
            return
        
        temp = self.head
        while temp.next.next:
            temp = temp.next

        temp.next = None
    def deletelastwithcount(self):
        if self.head is None:
            return

        if self.head.next is None:
            self.head = None
            return

        count = 0 
        temp = self.head
        while temp != None:
            count += 1
            temp = temp.next

        curr = self.head
        for i in range(count - 2):
            curr = curr.next

        curr.next = None

    def deletebyValue(self,val):
        if self.head is None:
            return

        if self.head.val == val:
            self.head = self.head.next
            return
        prev = self.head
        curr = self.head.next

        while curr != None:
            if curr.val == val:
                prev.next = curr.next
                return
            prev = curr
            curr = curr.next


        
            


li = LinkedList()

li.insert(10)

li.insert(20)

li.insert(10)

result = li.midNode()
li.deletelastwithcount()
li.printlist()
print(result)
        
       