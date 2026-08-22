// Implement a cache where many threads can read concurrently but a write needs exclusive
// access—the standard use case for std::shared_mutex. Use std::shared_lock for readers and
// std::unique_lock for writers. Know the tradeoff: shared_mutex has more overhead than a plain
// mutex, so it only pays off when reads dominate and critical sections aren't trivially short. 

#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <cassert>
#include <unordered_map>

class ConcurrentCache {
public:
    void write(int key, int value) {
        std::unique_lock lock(mtx_);
        data_[key] = value;
        return;
    }

    bool read(int key, int& out) {
        std::shared_lock lock(mtx_);
        auto it = data_.find(key);
        if (it == data_.end()) {
            return false;
        }
        out = it->second;
        return true;
    }

private:
    mutable std::shared_mutex mtx_; 
    std::unordered_map<int, int> data_;
};

int main() {
    ConcurrentCache cache;
    const int keys = 100;

    std::vector<std::thread> writers;
    for (int k = 0; k < keys; ++k)
        writers.emplace_back([&, k] { cache.write(k, k * k); });
    for (auto& t : writers) t.join();

    std::atomic<int> matched{0};
    std::vector<std::thread> readers;
    for (int r = 0; r < 20; ++r)
        readers.emplace_back([&] {
            for (int k = 0; k < keys; ++k) {
                int v;
                if (cache.read(k, v) && v == k * k) ++matched;
            }
        });
    for (auto& t : readers) t.join();

    assert(matched.load() == 20 * keys);
    std::cout << "PASS: matched = " << matched.load() << " ";
    return 0;
}