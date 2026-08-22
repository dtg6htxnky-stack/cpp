// Given the Order struct (price, an arrival sequence number, and quantity), write a comparator
// so that std::priority_queue<Order, std::vector<Order>, BidComparator> pops orders in strict
// price-time priority for the buy side: the highest price first, and when two orders share a
// price, the one with the lower (earlier) sequence number first. 

#include <iostream>
#include <cassert>
#include <queue>
#include <vector>
#include <cstdint>

struct Order {
    int64_t price;
    uint64_t seq; // arrival order, lower = earlier
    int64_t qty;
};

// TODO: implement a comparator so std::priority_queue exposes the order
// with the BEST price (highest) at top, and on a price tie, the order
// with the EARLIEST arrival (lowest seq).
struct BidComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price < b.price) {
            return true;
        } else if (a.price > b.price) {
            return false;
        } else if (a.seq > b.seq) {
            return true;
        } else {
            return false;
        }
    }
};

int main() {
    std::priority_queue<Order, std::vector<Order>, BidComparator> bids;
    bids.push({10050, 1, 100});
    bids.push({10075, 2, 50});
    bids.push({10075, 0, 30}); // same price, earlier seq should win
    bids.push({10025, 3, 200});

    assert(bids.top().price == 10075 && bids.top().seq == 0);
    bids.pop();
    assert(bids.top().price == 10075 && bids.top().seq == 2);
    bids.pop();
    assert(bids.top().price == 10050 && bids.top().seq == 1);
    bids.pop();
    assert(bids.top().price == 10025 && bids.top().seq == 3);
    bids.pop();
    assert(bids.empty()); // fully drained, nothing left behind

    // three orders tied at the same price: must pop in strict arrival order
    std::priority_queue<Order, std::vector<Order>, BidComparator> tie;
    tie.push({20000, 5, 1});
    tie.push({20000, 2, 1});
    tie.push({20000, 9, 1});
    assert(tie.top().seq == 2);
    tie.pop();
    assert(tie.top().seq == 5);
    tie.pop();
    assert(tie.top().seq == 9);
    tie.pop();
    assert(tie.empty());

    // a single-element queue must still report the right top and empty correctly
    std::priority_queue<Order, std::vector<Order>, BidComparator> single;
    single.push({15000, 0, 42});
    assert(single.top().price == 15000);
    single.pop();
    assert(single.empty());

    std::cout << "PASS\n";
    return 0;
}