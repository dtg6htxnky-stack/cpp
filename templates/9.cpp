#include <iostream>

auto adder = [](auto a, auto b) { return a + b; };

struct AdderStruct {
    template<typename A, typename B>
    auto operator()(A a, B b) const { return a + b; }
};

int main() {
    AdderStruct adderStruct;
    std::cout << adder(2, 3) << "\n";
    std::cout << adderStruct(2, 3) << "\n";
}