#include <iostream>

class Account
{
    int id;
    double rate;
    double balance;
    double interest = (balance * rate);

public:
    Account(int id, double balance, double rate): id(id), rate(rate), balance(balance) {}

    int getId()
    {
        return id;
    }

    int getBalance()
    {
        return balance;
    }

    int getInterest()
    {
        return interest;
    }
};

int main(){
  Account a(7, 1000.0, 0.05);
  std::cout << a.getId() << " " << a.getBalance() << " " << a.getInterest() << "\n";
}