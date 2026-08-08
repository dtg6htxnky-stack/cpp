#include <iostream>
#include <memory>

template<typename T>
using Ptr = std::unique_ptr<T>;

int main(){
  Ptr<int> p = std::make_unique<int>(42);
  std::cout << *p << "\n";
}