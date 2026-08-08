#include <iostream>
#include <utility>

struct Widget {
  Widget(int&) { std::cout << "ctor: lvalue\n"; }
  Widget(int const&) { std::cout << "ctor: const-lvalue\n"; }
  Widget(int&&) { std::cout << "ctor: rvalue\n"; }
};

template<typename T>
T myMakeUnique(T&& arg) {
    return T(std::forward<T>(arg));
}

int main(){
  int x = 5;
  const int cx = 10;
  auto a = myMakeUnique<Widget>(x);
  auto b = myMakeUnique<Widget>(cx);
  auto c = myMakeUnique<Widget>(20);
}