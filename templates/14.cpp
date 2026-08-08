#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

template<typename T>
void process(T arg) {
    if constexpr(std::is_integral<T>()) {
        std::cout << "arithmetic: " << arg << std::endl;
    } else if constexpr(std::is_same<T, std::string>()) {
        std::cout << "string: " << arg << std::endl;
    } else {
        std::cout << "container of size: " << arg.size() << std::endl;
    }
}

int main(){
  process(5);
  process(std::string("hi"));
  process(std::vector<int>{1,2,3});
}