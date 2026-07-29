#include <iostream>

template<typename T>
class PricerBase {
public:
    double price() const {
        return (static_cast<const T*>(this))->priceImpl();
    }
};

class FlatPricer : public PricerBase<FlatPricer> {
public:
    double priceImpl() const {
        return 100.0;
    };
};

class VirtualPricer : public PricerBase<FlatPricer> {
public:
    virtual double price() const {
        return 100.0;
    }
};

class SpreadPricer : public PricerBase<SpreadPricer> {
public:
    explicit SpreadPricer(double low, double high) : price_((high + low) / 2) { };

    int priceImpl() const {
        return price_;
    };

private:
    double price_;
};

template <typename T> 
int totalCost(const PricerBase<T>& p, int qty) {
    return p.price() * qty;
}

int main(){
    FlatPricer fp;
    SpreadPricer sp(99.5, 100.5);
    std::cout << fp.price() << " " << sp.price() << "\n";
    std::cout << totalCost(fp, 10) << " " << totalCost(sp, 4) << "\n";
    // FlatPricer is an empty class with no state but needs 1 byte to have own address.
    // VirtualPricer needs a vptr which is 8 bytes (64-bit system).
    std::cout << sizeof(FlatPricer) << " " << sizeof(VirtualPricer) << "\n";
}