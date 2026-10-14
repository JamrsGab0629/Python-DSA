class bank:
    def __init__ (self,name,balance):
        self._name = name
        self._balance = balance


    
    def getBalance(self):
        return self._balance



    def withdraw(self,value):
        if  self._balance < value or self._balance < 0: 
            raise ValueError("Balance was not enough")

        
        self._balance = self._balance - value

        return self._balance


    def deposit(self,value):
        if value < 0:
            raise ValueError("please deposit with more amount")


        self._balance = self._balance + value







b = bank("james",1000)
amount = b.getBalance()



b.deposit(1000)
amountNow = b.getBalance()

print(amountNow)

b.withdraw(500)
amountNow1 = b.getBalance()

print(amountNow1)

