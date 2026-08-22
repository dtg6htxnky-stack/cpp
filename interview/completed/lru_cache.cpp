// Design and implement a fixed-capacity key/value cache, LRUCache<K, V>, that evicts the
// least-recently-used entry when it's full. get(key, outValue) returns whether the key was
// present and, on a hit, counts as a 'use' of that entry. put(key, value) inserts or updates a
// key, counts as a use, and evicts the LRU entry first if the cache is already at capacity.
// Both operations must run in O(1) time, not O(log n) or O(capacity).

// 1. How would you make this thread-safe without serializing every get() behind one lock?
//    Get needs to be fast & not modify the list. It will retrieve data, if available, and
//    offload the list modification to a ring buffer that a separate worker will pick up.

// 2. How do you turn this into an LFU cache instead, and does O(1) still hold?
//    The node will need to track the number of uses. Then, the list will need to change to a
//    doubly linked list of doubly linked lists where the top-level list is responsible for
//    tracking the use buckets and the inner list tracks the individual nodes in the bucket.
//    When a node is used, it moves to the higher frequency bucket, allocating if needed. When
//    space is needed, the first node of the least used bucket is freed.

// 3. How would you add TTL-based expiry on top of this without breaking the O(1) bound?
//    The node will need to track its expiry. Nodes do not need to be deleted as soon as their
//    time expires. On get, we will check if the node we are interested in is expiring. If so,
//    we will treat it as if it was a cache miss. When nodes are created, based on their TTL,
//    they should be added to a separate structure which buckets nodes based on their TTL. At a
//    given frequency, another worker thread will prune expired buckets.

// 4. What's the memory overhead per entry compared to a plain hash map, and does that matter
//    here? 
//    LRU has an overhead of 16 bytes (2 pointers) and worst case of 14 bytes for padding
//    depending on the key/value types. This matters depending on priorities of the application
//    and the underlying data being stored.

#include <iostream>
#include <cassert>
#include <string>
#include <unordered_map>

template <typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(size_t capacity) {
        assert(capacity != 0);
        
        capacity_ = 0;
        max_capacity_ = capacity;
        head_ = { K(), V(), &head_, &head_ };
    }

    bool get(const K& key, V& outValue) {
        if (map_.find(key) == map_.end()) {
            return false;
        }

        Node* retrieved = map_.at(key);

        make_recent(retrieved);

        outValue = retrieved->value;
        return true;
    }

    void put(const K& key, const V& value) {
        // Update.
        if (map_.find(key) != map_.end()) {
            Node* to_update = map_.at(key);
            to_update->value = value;

            make_recent(to_update);

            return;
        }

        // Remove.
        if (capacity_ == max_capacity_) {
            Node* to_delete = head_.prev;
            to_delete->prev->next = &head_;
            head_.prev = to_delete->prev;

            map_.erase(to_delete->key);
            delete to_delete;

            capacity_ -= 1;
        }

        // Insert.
        Node* n = new Node{ key, value, head_.next, &head_ };
        head_.next->prev = n;
        head_.next = n;
        map_.insert({key, n});

        capacity_ += 1;
    }

    void print() {
        Node* it = head_.next;
        while(it != &head_) {
            std::cout << it->key << " ";
            it = it->next;
        }
        std::cout << std::endl;
    }

private:
    struct Node {
        K key;
        V value;
        Node* next;
        Node* prev;
    };

    void make_recent(Node* n) {
        // Relink the neighbors.
        n->prev->next = n->next;
        n->next->prev = n->prev;

        // Relink new.
        n->next = head_.next;
        n->prev = &head_;

        // Update head.
        head_.next->prev = n;
        head_.next = n;
    }

    size_t capacity_;
    size_t max_capacity_;
    Node head_;
    std::unordered_map<K, Node*> map_;
};

int main() {
    LRUCache<int, std::string> cache(2);
    cache.put(1, "a");
    cache.put(2, "b");
    std::string v;
    cache.print();
    assert(cache.get(1, v) && v == "a"); // touches 1, order becomes: 2, 1
    cache.print();
    cache.put(3, "c"); // evicts 2 (least recently used)
    assert(!cache.get(2, v));
    assert(cache.get(3, v) && v == "c");
    assert(cache.get(1, v) && v == "a");
    cache.print();
    std::cout << "PASS\n";
    return 0;
}