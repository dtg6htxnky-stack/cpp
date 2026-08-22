#include <vector>
#include <iostream>
#include <cassert>
#include <cstddef>

struct IntrusiveListHook { 
    IntrusiveListHook* prev = nullptr; 
    IntrusiveListHook* next = nullptr; 
};

struct Order {
    long id;
    IntrusiveListHook hook;
};

class OrderList {
public:
    OrderList() { sentinel_.next = &sentinel_; sentinel_.prev = &sentinel_; };

    void push_back(Order& o) {
        IntrusiveListHook* order_hook = nullptr;
        IntrusiveListHook* last = nullptr;

        order_hook = &o.hook;
        last = sentinel_.prev; // sentinel

        // Set up new node.
        order_hook->prev = last; 
        order_hook->next = &sentinel_;

        // Link new node.
        last->next = order_hook;
        sentinel_.prev = order_hook;

        return;
    }

    void erase(Order& o) {
        IntrusiveListHook* order_hook;

        order_hook = &o.hook;

        // Set up existing list.
        order_hook->prev->next = order_hook->next;
        order_hook->next->prev = order_hook->prev;

        // Delete node.
        order_hook->prev = nullptr;
        order_hook->next = nullptr;
    }

    void print() const {
        const IntrusiveListHook* last = sentinel_.next;
        Order* current_order;

        while(last != &sentinel_) {
            current_order = (Order*)((char*)last - offsetof(Order, hook));
            std::cout << current_order->id << " ";
            last = last->next;
        }
        std::cout << std::endl;
    }

    template <typename F> 
    void for_each(F f) const {
        const IntrusiveListHook* iter = sentinel_.next;

        while (iter != &sentinel_) {
            f(*(Order*)((char*)iter - offsetof(Order, hook)));
            iter = iter->next;
        }

        return;
    }
private:
    IntrusiveListHook sentinel_;
};

int main() {
    Order a{1}, b{2}, c{3};
    OrderList list;
    list.push_back(a); list.push_back(b); list.push_back(c);

    std::vector<long> ids;
    list.for_each([&](const Order& o) { ids.push_back(o.id); });
    assert((ids == std::vector<long>{1, 2, 3}));

    list.erase(b);
    ids.clear();
    list.for_each([&](const Order& o) { ids.push_back(o.id); });
    assert((ids == std::vector<long>{1, 3}));

    std::cout << "PASS\n";
}