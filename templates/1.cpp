#include <iostream>

template<typename T>
T myMax(T a, T b)
{
    if (a > b) {
        return a;
    }

    return b;
}

int main(){
  std::cout << myMax(3, 7) << '\n';
  std::cout << myMax(3.5, 2.25) << '\n';
  std::cout << myMax(std::string("apple"), std::string("banana")) << '\n';
  std::cout << myMax<double>(3, 3.5) << '\n';
}