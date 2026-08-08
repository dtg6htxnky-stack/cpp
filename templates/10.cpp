#include <iostream>

int counter = 0;
int& getRef(int a){ return counter += a; }

template<typename T, typename... Args>
decltype(auto) invoke(T&& f, Args... args) {
    return f(std::forward<Args>(args)...);
}

int main(){
  invoke(getRef, 10) = 42;
  std::cout << counter << "\n";
}