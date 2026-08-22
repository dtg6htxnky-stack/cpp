#include <atomic>
#include <thread>
#include <iostream>
#include <cassert>

template <typename T, size_t Capacity>
class SPSCQueue {
public:
    bool push(const T& v); // TODO
    bool pop(T& out);      // TODO
private:
    T buf_[Capacity];
    std::atomic<size_t> head_{0}; // consumer reads here
    std::atomic<size_t> tail_{0}; // producer writes here
};

int main() {
    SPSCQueue<int, 1024> q;
    const int N = 10'000'000;

    std::thread producer([&] {
        for (int i = 0; i < N; ++i) while (!q.push(i)) {}
    });

    std::thread consumer([&] {
        for (int i = 0; i < N; ++i) {
            int v;
            while (!q.pop(v)) {}
            assert(v == i); // must arrive in order, none dropped/duplicated
        }
    });

    producer.join();
    consumer.join();
    std::cout << "PASS\n";
}