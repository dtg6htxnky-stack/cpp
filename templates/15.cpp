#include <iostream>
#include <vector>

int main(){
  Box b{42};
  std::cout << b.value << "\n";

  std::vector v1{1};
  std::vector<int> v2(1);
  std::cout << v1.size() << " " << v1[0] << "\n";
  std::cout << v2.size() << " " << v2[0] << "\n";
}