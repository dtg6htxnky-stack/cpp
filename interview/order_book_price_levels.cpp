// Maintain aggregate quantity per price level for one side of a book with O(log n) add/cancel
// and O(1) best-price lookup. Prices are integer ticks, not doubles—never key a price level off
// floating point. Use an ordered map with the comparator flipped for bids vs. asks.

#include <iostream>
#include <cassert>
#include <cstdint>

// Price expressed as integer ticks to avoid floating point error
class PriceLevels {
public:
    void addQty(int64_t priceTicks, int64_t qty);    // TODO
    void cancelQty(int64_t priceTicks, int64_t qty);  // TODO
    bool best(int64_t& priceTicks, int64_t& qty) const; // TODO: best = highest for bids

private:
    
};

int main() {
    PriceLevels bids;
    bids.addQty(10050, 100);
    bids.addQty(10075, 50);
    bids.addQty(10025, 200);

    int64_t p, q;
    assert(bids.best(p, q) && p == 10075 && q == 50);

    bids.cancelQty(10075, 50); // level empties, removed
    assert(bids.best(p, q) && p == 10050 && q == 100);

    bids.addQty(10050, 25);
    assert(bids.best(p, q) && p == 10050 && q == 125);

    std::cout << "PASS\n";
    return 0;
}