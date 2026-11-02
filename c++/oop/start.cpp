#include <iostream>
#include <stdexcept>

class Bank {
    private:
    double balance;
    
    public:
    Bank(double balance){

        if (balance < 0){
            throw std::invalid_argument("balance cant be 0 or less than 0 ");
        }
        this->balance = balance;
    }
    
    double getBalance() const{
        if (this->balance < 0 || this->balance == 0){
            throw std::invalid_argument("value can not be negative or lower");
        }
        return balance;
    }

    void withdraw(double amount){
        if (this->balance < amount){
            throw std::invalid_argument("Your balance is less than 0");
        }

        this->balance -= amount;
        
    }
    void deposit(double amount){
        if (amount < 0 || amount == 0){
            throw std::invalid_argument("amount cant be less than 0 or 0");
        }
        this->balance = this->balance + amount;
    }
     
    
};
int main(){
Bank b(0);
b.deposit(1000);
b.withdraw(500);

int balance = b.getBalance();

std::cout << balance;
}