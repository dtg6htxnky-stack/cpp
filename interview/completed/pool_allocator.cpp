// Implement OrderPool, a fixed-capacity allocator for Order objects that avoids calling the
// general-purpose new/malloc on the hot path. The constructor pre-allocates storage for exactly
// capacity objects. acquire() hands back a pointer to a live, default-constructed Order or
// nullptr if the pool is exhausted; release(Order*) destroys that object and returns its
// storage to the pool for reuse. Repeated acquire/release calls must not grow the pool's memory
// footprint, and a slot freed by release must be eligible for reuse by a later acquire.

// 1. Why can't you just use a std::vector with an index-based free list here — what does that
//    get wrong? 
//    We will not be able to keep track of the memory that is freed in O(1) time. To make sure
//    freed memory would get reused, O(N) complexity is required to check slots until we find
//    the free one.

// 2. How would you make acquire()/release() safe to call from multiple threads without
//    serializing every call behind one lock?
//    Make the free node pointer into an atomic value and use atomic compare and exchange to
//    allow for lock-free concurrency. To guard against ABA, the node structure will need to
//    keep a monotonically increasing tag.

// 3. What happens if a caller calls release() twice on the same pointer, or uses a pointer
//    after releasing it — how would you guard against that in debug builds?
//    It would create a loop in the list. We would need to guard that the caller is only freeing
//    memory we allocated and that the memory being freed is currently allocating. This would
//    require a runtime range check on the address and a debug-only member variable on the
//    structure to keep track of whether it is currently allocated.

// 4. How would you extend this to variable-sized allocations (a general-purpose pool
//    allocator)?
//    I would have multiple pools each with a unique, fixed-size node. Then, a top-level
//    orchestrator would redirect the allocation to the pool which has nodes of the closest
//    size, rounded up to the next node.

#include <iostream>
#include <cassert>
#include <cstdint>
#include <cstddef>

struct Order {
    int64_t id;
    int64_t price;
    int64_t qty;
};

class OrderPool {
public:
    explicit OrderPool(size_t capacity) {
        OrderNode* it;

        buff_ = new OrderNode[capacity]();

        // Create initial linkage.
        for (size_t i = 0; i < capacity - 1; ++i) {
            buff_[i].next = &buff_[i + 1];

        }
        buff_[capacity-1].next = nullptr;
        free_ = buff_;
    }

    ~OrderPool() {
        delete[] buff_;
    }

    Order* acquire() {
        Order* returned;

        // No more space.
        if (free_ == nullptr) {
            return nullptr;
        }

        // Use the front node.
        returned = (Order*)&free_->o;

        // Move to the next.
        free_ = free_->next;

        return returned;
    }

    void release(Order* o) {
        OrderNode* node;

        node = (OrderNode*)o;

        // Mark as free.
        node->next = free_;
        free_ = node;

        return;
    }

private:
    struct OrderNode {
        Order o;
        OrderNode* next;
    };

    OrderNode* free_;
    OrderNode* buff_;
};

int main() {
    OrderPool pool(4);
    Order* a = pool.acquire();
    Order* b = pool.acquire();
    a->id = 1; b->id = 2;
    pool.release(a);
    Order* c = pool.acquire(); // should reuse a's slot (LIFO free list)
    assert(c == a);
    (void)b;
    std::cout << "PASS\n";
    return 0;
}