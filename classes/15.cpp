#include <iostream>
#include <map>
#include <memory>

class OrderBook {
public:
    OrderBook();
    ~OrderBook();
    OrderBook(OrderBook&&) noexcept;
    OrderBook& operator=(OrderBook&&) noexcept;
    OrderBook(const OrderBook&) = delete;
    OrderBook operator=(const OrderBook&) = delete;

    void addOrder(double bid, int size);
    double bestBid(void) const;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

struct OrderBook::Impl {
    std::map<double, int> priceToQuantity;
};

OrderBook::OrderBook() : pimpl(std::make_unique<OrderBook::Impl>()) { }
OrderBook::~OrderBook() = default;
OrderBook::OrderBook(OrderBook&&) noexcept = default;
OrderBook& OrderBook::operator=(OrderBook&&) noexcept = default;

void OrderBook::addOrder(double price, int qty) {
    pimpl->priceToQuantity[price] = qty;
}

double OrderBook::bestBid() const {
    return pimpl->priceToQuantity.rbegin()->first;
}

int main(){
  OrderBook book;
  book.addOrder(101.5, 100);
  book.addOrder(102.25, 50);
  std::cout << book.bestBid() << "\n";
  OrderBook moved(std::move(book));
  std::cout << moved.bestBid() << "\n";
}