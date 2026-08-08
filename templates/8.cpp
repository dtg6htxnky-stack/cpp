#include <iostream>

template<typename T, typename U>
decltype(auto) add(T x, U y) {
    return x + y;
}

int main(){
  std::cout << add(2, 3.5) << "\n";
}