#include <iostream>
#include <cstddef>

struct TickBad {
    char flag;
    double price;
    int qty;
};

struct TickGood {
    double price;
    int qty;
    char flag;
};

struct alignas(16) TickAligned {
    double price;
    int qty;
};

int main(){
  std::cout << sizeof(TickBad) << "\n";
  std::cout << offsetof(TickBad, flag) << " " << offsetof(TickBad, price) << " " << offsetof(TickBad, qty) << "\n";
  std::cout << sizeof(TickGood) << "\n";
  std::cout << offsetof(TickGood, price) << " " << offsetof(TickGood, qty) << " " << offsetof(TickGood, flag) << "\n";
  std::cout << sizeof(TickAligned) << " " << alignof(TickAligned) << "\n";
}