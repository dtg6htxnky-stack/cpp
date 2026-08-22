// Implement Spinlock, a mutual-exclusion lock built only from std::atomic_flag — no std::mutex
// or other OS synchronization primitive is allowed. lock() must not return until exclusive
// ownership is acquired, and busy-waits rather than putting the thread to sleep. unlock()
// releases ownership. Concurrent increments to a shared counter guarded by lock()/unlock() from
// many threads must produce the exact expected total with no lost updates. 

// 1. What memory_order does each atomic operation actually need, and what could go wrong with
//    the wrong choice? 
//    memory_order_relaxed: Only guarantees atomicity, reordering is allowed.
//    memory_order_acquire/release: Guarantees atomicity and order of preceeding (release) or
//                                  subsequent (acquire) instructions.
//    memory_order_seq_cst: Single global order across all threads.
//    The wrong choice may mean that the counter increment may be reordered, meaning that the
//    atomic flag will offer no synchronization guarantees.

#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include <cassert>

class Spinlock {
public:
    void lock() {
        while (flg_.test_and_set(std::memory_order_acquire)) {
            // Waiting.
        }

        return;
    }

    void unlock() {
        flg_.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flg_ = ATOMIC_FLAG_INIT;
};

int main() {
    Spinlock lock;
    long counter = 0;
    const int threads = 8, iters = 100000;

    std::vector<std::thread> pool;
    for (int t = 0; t < threads; ++t)
        pool.emplace_back([&] {
            for (int i = 0; i < iters; ++i) {
                lock.lock();
                ++counter; // unprotected ++ would race
                lock.unlock();
            }
        });

    for (auto& th : pool) th.join();
    assert(counter == (long)threads * iters);
    std::cout << "PASS: counter = " << counter << "\n";
    return 0;
}