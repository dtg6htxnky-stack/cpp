#include <iostream>
#include <string>
#include <stdexcept>

class Transaction
{
public:
    Transaction(std::string name): name(name) {
        std::cout << "begin " << name << "\n";
    };

    ~Transaction() {
        if (hasCommitted) {
            return;
        }

        std::cout << "rollback " << name << "\n";
    }

    void commit() {
        hasCommitted = true;
        std::cout << "commit " << name << "\n";
    }

private:
    std::string name;
    bool hasCommitted = false;
};

void placeOrder(bool shouldFail) {
  Transaction t("order-42");
  if (shouldFail) throw std::runtime_error("risk check failed");
  t.commit();
}
int main(){
  try {
    placeOrder(false);
  } catch (const std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
  }
  try {
    placeOrder(true);
  } catch (const std::exception& e) {
    std::cout << "caught: " << e.what() << "\n";
  }
}