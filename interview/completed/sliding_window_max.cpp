// Implement SlidingWindowMax, constructed with a fixed window size k. push(value) feeds one new
// value from a stream; once more than k values have been pushed, the oldest one automatically
// falls out of the window. max() returns the maximum value currently in the window. Both
// operations must run in amortized O(1) time regardless of k — recomputing the max over the
// whole window on every push is not acceptable. 

// 1. Why is this amortized O(1) per push even though a single push can pop many elements from
//    the internal structure at once?
//    Because each element can only be popped once. ((n + 1 + n) / n) ~= O(1)

// 2. How would you extend this to report the sliding window minimum at the same time as the
//    maximum?
//    Maintain a second deque. The max-deque can't be reused since we pop smaller numbers than
//    the max.

// 3. Where would this pattern show up in a trading system (e.g. rolling high/low over the last
//    N ticks)?
//    Breakout detection, or risk detection.

// 4. How would you adapt this if you needed to query the max over an arbitrary past window, not
//    just the current one?
//    Depends on which operation (read or write) needs to be optimized. If both matter, I'd use
//    a segment tree. If performant reads are more important, I'd keep a pre-computed sparse 2D
//    table.

#include <cstddef>
#include <deque>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

class SlidingWindowMax {
public:
    explicit SlidingWindowMax(size_t k) : window_(k) {
        index_ = 0;
    }

    void push(int value) {
        int current;

        index_ += 1;

        while ((!order_.empty()) && order_.back().first < value) {
            order_.pop_back();
        }

        order_.push_back(std::pair<int, size_t>(value, index_));

        while (((index_ - order_.front().second) >= window_)) {
            order_.pop_front();
        }

        return;
    }

    int max() const {
        return order_.front().first;
    }

private:
    const size_t window_;
    size_t index_;
    std::deque<std::pair<int, size_t>> order_;
};

int main() {
    SlidingWindowMax w(3);
    w.push(1); w.push(3); w.push(-1);
    assert(w.max() == 3); // window: [1,3,-1]
    w.push(-3);
    assert(w.max() == 3); // window: [3,-1,-3]
    w.push(5);
    assert(w.max() == 5); // window: [-1,-3,5]
    w.push(3);
    assert(w.max() == 5); // window: [-3,5,3]
    w.push(6);
    assert(w.max() == 6); // window: [5,3,6]

    // window size 1: max is always exactly the most recent value
    SlidingWindowMax w1(1);
    w1.push(10);
    assert(w1.max() == 10);
    w1.push(2);
    assert(w1.max() == 2);
    w1.push(2);
    assert(w1.max() == 2);

    // strictly increasing stream: every push should push the max forward,
    // and old smaller values must fall out of the window on schedule
    SlidingWindowMax inc(4);
    for (int i = 1; i <= 10; ++i) {
        inc.push(i);
        assert(inc.max() == i); // increasing values are always the new max
    }

    // strictly decreasing stream: the max must correctly expire once it
    // falls outside the window, even though it was the largest value seen
    SlidingWindowMax dec(3);
    dec.push(10); dec.push(9); dec.push(8);
    assert(dec.max() == 10); // window: [10,9,8]
    dec.push(7);
    assert(dec.max() == 9); // window: [9,8,7], 10 has expired
    dec.push(6);
    assert(dec.max() == 8); // window: [8,7,6]

    // randomized stress test against an O(k) brute-force recompute
    const size_t K = 5;
    SlidingWindowMax stress(K);
    std::vector<int> history;
    unsigned seed = 999;
    for (int i = 0; i < 500; ++i) {
        seed = seed * 1103515245u + 12345u;
        int v = (int)(seed % 200) - 100;
        stress.push(v);
        history.push_back(v);
        size_t start = history.size() >= K ? history.size() - K : 0;
        int bruteMax = history[start];
        for (size_t j = start; j < history.size(); ++j) bruteMax = std::max(bruteMax, history[j]);
        assert(stress.max() == bruteMax);
    }

    std::cout << "PASS\n";
    return 0;
}