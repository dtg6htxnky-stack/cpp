#include <atomic>
#include <cstddef>
#include <thread>
#include <vector>
#include <iostream>
#include <cassert>

template <typename T, size_t Capacity>
class SPSCRingBuffer {
public:
    SPSCRingBuffer(void) : producer_(0), consumer_(0) { };

    bool push(const T& item) {
        const size_t current_producer = producer_.load();
        const size_t next_producer = current_producer + 1;
        const size_t current_consumer = consumer_.load();
        const size_t next_consumer = consumer_.load() + 1;

        if (next_consumer == current_producer) {
            return false;
        }

        buf_[producer_] = item;
        producer_ = ((producer_ + 1) & (Capacity - 1));

        return true;
    }

    bool pop(T& item) {
        if (isEmpty()) {
            return false;
        }

        buf_[consumer_] = item;
        consumer_ = ((consumer_ + 1) & (Capacity - 1));

        return true;
    }

    bool isFull(void) const {
        // Sacrifice one spot to distinguish between full and empty.
        return ((consumer_ + 1) == producer_);
    }

    bool isEmpty(void) const {
        return (consumer_ == producer_);
    }

private:
    // Capacity is a bitmask, must be a power of two.
    T buf_[(Capacity - 1)];

    // Prevent false sharing.
    // Consumer.
    alignas(64) std::atomic<size_t> consumer_;
    // Producer.
    alignas(64) std::atomic<size_t> producer_;
};

int main() {
    SPSCRingBuffer<int, 1024> buffer;
    const int N = 100000;
    long long expectedSum = (long long)N * (N - 1) / 2;
    long long actualSum = 0;

    std::thread producer([&]() {
        for (int i = 0; i < N; ++i) {
            while (!buffer.push(i)) { 
                std::cout << "Spinning " << i << " " << std::endl;
             }
        }
    });

    std::thread consumer([&]() {
        int received = 0;
        int value;
        while (received < N) {
            if (buffer.pop(value)) {
                actualSum += value;
                ++received;
            }
        }
    });

    producer.join();
    consumer.join();

    assert(actualSum == expectedSum);
    std::cout << "PASS: sum = " << actualSum << "\n";
    return 0;
}