#include <iostream>
#include <vector>

struct TickAoS {
    double price;
    long volume;
};

double vwapAoS(const std::vector<TickAoS>& ticks) {
    double weightedSum = 0.0;
    long totalVolume = 0;
    for (const auto& t : ticks) {
        weightedSum += t.price * t.volume;
        totalVolume += t.volume;
    }
    return weightedSum / totalVolume;
}

class TickBookSoA {
public:
    void add(double price, long volume) {
        prices_.push_back(price);
        volumes_.push_back(volume);
    }

    double vwap() const {
        double weightedSum = 0.0;
        long totalVolume = 0;
        for (size_t i = 0; i < prices_.size(); ++i) {
            weightedSum += prices_[i] * volumes_[i];
            totalVolume += volumes_[i];
        }
        return weightedSum / totalVolume;
    }

    size_t count() const { return prices_.size(); }

private:
    std::vector<double> prices_;
    std::vector<long> volumes_;
};

int main(){
  std::vector<TickAoS> aos{{101.5, 100}, {101.75, 200}, {101.25, 150}};
  TickBookSoA soa;
  soa.add(101.5, 100); soa.add(101.75, 200); soa.add(101.25, 150);
  std::cout << vwapAoS(aos) << "\n";
  std::cout << soa.vwap() << "\n";
  std::cout << sizeof(TickAoS) * aos.size() << "\n";
  std::cout << soa.count() << "\n";
}