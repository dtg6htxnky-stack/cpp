// Implement MedianTracker, which reports the median of every number seen so far as new numbers
// arrive one at a time in arbitrary order (they must not be assumed sorted, and the full
// history cannot simply be re-sorted on every query). addNumber(num) must run in O(log n);
// getMedian() must run in O(1). For an even count of numbers, the median is the average of the
// two middle values. 

#include <iostream>
#include <cassert>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

class MedianTracker {
public:
    void addNumber(double num) {

    }

    double getMedian() const {
        return median;
    }

private:
    std::map<
    double median;
};

int main() {
    MedianTracker t;
    t.addNumber(5);
    assert(std::abs(t.getMedian() - 5.0) < 1e-9); // single element is its own median

    t.addNumber(2);
    assert(std::abs(t.getMedian() - 3.5) < 1e-9); // (2+5)/2
    t.addNumber(8);
    assert(std::abs(t.getMedian() - 5.0) < 1e-9); // middle of 2,5,8
    t.addNumber(1);
    assert(std::abs(t.getMedian() - 3.5) < 1e-9); // (2+5)/2 of 1,2,5,8
    t.addNumber(9);
    assert(std::abs(t.getMedian() - 5.0) < 1e-9); // middle of 1,2,5,8,9

    // duplicate values must be handled like any other value, not deduplicated
    MedianTracker dup;
    for (double v : {4.0, 4.0, 4.0, 4.0}) dup.addNumber(v);
    assert(std::abs(dup.getMedian() - 4.0) < 1e-9);
    dup.addNumber(100.0);
    assert(std::abs(dup.getMedian() - 4.0) < 1e-9); // middle of 4,4,4,4,100 is 4

    // strictly descending input order must rebalance correctly, not just
    // ascending or randomly-ordered input
    MedianTracker desc;
    std::vector<double> descInput = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<double> seenSoFar;
    for (double v : descInput) {
        desc.addNumber(v);
        seenSoFar.push_back(v);
        std::vector<double> sorted = seenSoFar;
        std::sort(sorted.begin(), sorted.end());
        size_t n = sorted.size();
        double expected = (n % 2 == 1) ? sorted[n / 2] : (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
        assert(std::abs(desc.getMedian() - expected) < 1e-9);
    }

    // randomized stress test: cross-check against a brute-force sort after
    // every insertion, across a much longer and less structured stream
    MedianTracker stress;
    std::vector<double> seen;
    unsigned seed = 12345;
    for (int i = 0; i < 500; ++i) {
        seed = seed * 1103515245u + 12345u; // simple deterministic PRNG, no <random> needed
        double v = (double)(seed % 10000);
        stress.addNumber(v);
        seen.push_back(v);
        std::vector<double> sorted = seen;
        std::sort(sorted.begin(), sorted.end());
        size_t n = sorted.size();
        double expected = (n % 2 == 1) ? sorted[n / 2] : (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
        assert(std::abs(stress.getMedian() - expected) < 1e-6);
    }

    std::cout << "PASS\n";
    return 0;
}