#include <iostream>

template<typename... T>
constexpr int sum(T... args) {
  return (args + ...);
}

template<typename... T>
constexpr bool allTrue(T... args) {
  return (args && ...);
}

template<typename... T>
constexpr void printAll(T... args) {
  ((std::cout << args << " "), ...) << std::endl;
}

int main(){
  std::cout << sum(1,2,3,4) << "\n";
  printAll(1, "two", 3.0);
  std::cout << std::boolalpha << allTrue(true,true,true) << " " << allTrue(true,false,true) << "\n";
}