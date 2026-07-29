#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <compare>

class Money {
public:
    explicit Money(int cent) : cent_(cent) {};

    Money& operator+=(const Money& other) {
        this->cent_ += other.cent_;
        return *this;
    }

    friend Money operator+(Money& lhs, const Money& rhs) {
        return Money(lhs.cent_ + rhs.cent_);
    }

    auto operator<=>(const Money&) const = default;
    
    friend std::ostream& operator<<(std::ostream& out, const Money& compare) {
        return out << (compare.cent_ / 100) << "." << (compare.cent_ % 100);
    }

private:
    int cent_;
};

int main(){
  Money a(1050), b(275);
  std::cout << (a + b) << "\n";
  a += b;
  std::cout << a << "\n";
  std::cout << std::boolalpha << (Money(500) < Money(600)) << " " << (Money(500) == Money(500)) << "\n";
  std::vector<Money> ms{Money(300), Money(100), Money(200)};
  std::sort(ms.begin(), ms.end());
  for (const auto& m : ms) std::cout << m << " ";
  std::cout << "\n";
}