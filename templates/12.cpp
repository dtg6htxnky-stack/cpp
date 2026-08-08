#include <iostream>

constexpr int factorial(int n) {
    if (n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main(){
  static_assert(factorial(5) == 120);
  std::cout << factorial(5) << "\n";
}