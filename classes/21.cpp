#include <cstddef>
#include <iostream>
#include <utility>

struct Order {
    int id;
    double price;
    Order(int id, double price) : id(id), price(price) {}
};

template <typename T, size_t N>
class ObjectPool {
public:
    ObjectPool() {
        for (size_t i = 0; i < N; ++i) freeList_.push_back(i);
    }

    template <typename... Args>
    T* acquire(Args&&... args) {
        if (freeList_.empty()) return nullptr;
        size_t slot = freeList_.back();
        freeList_.pop_back();
        T* ptr = reinterpret_cast<T*>(&storage_[slot]);
        new (ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    void release(T* obj) {
        obj->~T();      // manual destructor call
        size_t slot = reinterpret_cast<Slot*>(obj) - storage_;
        freeList_.push_back(slot);
    }

    size_t available() const { return freeList_.size(); }

private:
    using Slot = std::aligned_storage_t<sizeof(T), alignof(T)>;
    Slot storage_[N];
    std::vector<size_t> freeList_; // LIFO free list
};

int main(){
  ObjectPool<Order,4> pool;
  std::cout << pool.available() << "\n";
  Order* a = pool.acquire(1, 100.5);
  Order* b = pool.acquire(2, 200.25);
  std::cout << pool.available() << " " << a->id << " " << b->price << "\n";
  pool.release(a);
  std::cout << pool.available() << "\n";
  Order* c = pool.acquire(3, 50.0);
  std::cout << (c == a) << " " << c->id << "\n";
  pool.release(b);
  pool.release(c);
  std::cout << pool.available() << "\n";
}