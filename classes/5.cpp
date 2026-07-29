#include <iostream>

class Price
{
public:
    Price() : Price(0LL) {}
    Price(double dollars): Price(static_cast<long long>(dollars * 10000)) {};

    static Price fromTicks(long long nTicks) 
    {
        return Price(nTicks);
    };

    long long ticks() const
    {
        return this->ticksCount;
    }

    double dollars() const
    {
        return (this->ticksCount / 10000.0);
    }

private:
    Price(long long nTicks): ticksCount(nTicks) {};
    long long ticksCount = 0;
};

void quote(const Price& p) { std::cout << p.dollars() << "\n"; }
int main(){
  Price p1;
  Price p2(3.5);
  Price p3 = Price::fromTicks(125000);
  std::cout << p1.ticks() << " " << p2.ticks() << " " << p3.ticks() << "\n";
  quote(p2);
  quote(Price(2.25));
}