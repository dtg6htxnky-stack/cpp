#include <iostream>
#include <iomanip>

template<typename T>
constexpr T pi = T(3.1415);

template<typename T, typename U>
constexpr bool is_same_v = false;

template<typename T>
constexpr bool is_same_v<T, T> = true;

int main(){
  std::cout << std::fixed << std::setprecision(2);
  std::cout << pi<float> << "\n";
  std::cout << pi<double> << "\n";
  std::cout << std::boolalpha << is_same_v<int,int> << " " << is_same_v<int,double> << "\n";
}