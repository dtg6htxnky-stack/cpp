#include <atomic>
#include <cassert>
#include <stdint.h>
#include <thread>

std::atomic<uint64_t> flags(0);

void worker_thread(uint8_t bit_position) {
    uint64_t expected;
    uint64_t desired;
    bool success;
   
    expected = flags.load(std::memory_order_relaxed);
    success = false;
    while (!success) {
        // Calculate desired value using the latest snapshot.
        desired = expected | (1ULL << bit_position);
        
        // Atomically swap. On failure, the expected value will
        // be updated with the current value.
        success = flags.compare_exchange_strong(expected,
                                                desired,
                                                std::memory_order_relaxed,
                                                std::memory_order_relaxed);
    }
}

int main(void) {
  // Create two threads that each attempt to modify the flags value by
  // setting a unique bit.
  std::thread thread1(worker_thread, 0);
  std::thread thread2(worker_thread, 1);

  thread1.join();
  thread2.join();

  // Both bits must be set.
  assert(flags.load() == ((1ULL << 0) | (1ULL << 1)));

  return 0;
}