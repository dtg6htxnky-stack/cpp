#include <iostream>
#include <string>

template<typename T>
struct Loggable {
    void log(const std::string& msg) const {
        std::cout << "[LOG] " << msg << "\n";
    }
};

template<typename T>
struct Comparable {
    bool sameAs(const T& other) const {
        return (static_cast<const T*>(this))->key() == other.key();
    }
};

class Order : public Loggable<Order>
            , public Comparable<Order> 
{
public:
    explicit Order(int key) : key_(key) {}
    int key() const { return key_; }

private:
    int key_;
};


// Requires 1 byte to be addressable.
class Empty {

};

// Requires 4 bytes for 32-bit int, base class can use same address as child.
class WithBaseEBO : public Empty {
    int value;
};

// Requires 1 byte for e to be addressable, 3 bytes for padding, and 4 bytes for
// 32-bit int.
class WithMember {
    Empty e;
    int value;
};

int main(){
  Order o1(42), o2(42), o3(7);
  o1.log("created order");
  std::cout << std::boolalpha << o1.sameAs(o2) << " " << o1.sameAs(o3) << "\n";
  std::cout << sizeof(Empty) << " " << sizeof(WithBaseEBO) << " " << sizeof(WithMember) << "\n";
}