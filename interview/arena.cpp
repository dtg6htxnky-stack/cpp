#include <cstddef>
#include <new>
#include <chrono>
#include <iostream>

class Arena {
public:
    explicit Arena(size_t bytes) : buf_(new char[bytes]), size_(bytes), offset_(0) {}
    ~Arena() { delete[] buf_; }
    void* allocate(size_t n) {
        void* value;

        if (offset_ + n > size_) {
            return nullptr;
        }

        value = &buf_[offset_];
        offset_ += n;

        return value;
    }
    void reset() { offset_ = 0; }
private:
    char* buf_; 
    size_t size_; 
    size_t offset_;
};

struct Order { double price; int qty; };

int main() {
    const long N = 1'000'000;
    Arena arena(sizeof(Order) * N);

    auto t0 = std::chrono::steady_clock::now();
    for (long i = 0; i < N; ++i) {
        void* mem = arena.allocate(sizeof(Order));
        new (mem) Order{1.0, 1};
    }
    auto t1 = std::chrono::steady_clock::now();
    std::cout << "arena: " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms\n";

    auto t2 = std::chrono::steady_clock::now();
    for (long i = 0; i < N; ++i) { Order* o = new Order{1.0, 1}; delete o; }
    auto t3 = std::chrono::steady_clock::now();
    std::cout << "new/delete: " << std::chrono::duration<double, std::milli>(t3 - t2).count() << " ms\n";
    std::cout << "PASS\n";
}