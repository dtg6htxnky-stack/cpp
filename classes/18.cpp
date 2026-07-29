#include <cstddef>
#include <iostream>
#include <vector>

class Portfolio {
public:
    void add(double v) {
        positions_.push_back(v);
    }

    class Iterator {
    public:
        explicit Iterator(const Portfolio* portfolio, size_t index) : portfolio_(portfolio), index_(index) {}

        Iterator& operator++() {
            ++index_;
            return *this;
        }

        double operator*() const {
            return portfolio_->positions_[index_];
        }

        bool operator!=(const Iterator& other) const {
            return index_ != other.index_;
        }

    private:
        size_t index_;
        const Portfolio* portfolio_;
    };

    friend class PortfolioAuditor;

    Iterator begin() const { return Iterator(this, 0); }
    Iterator end() const { return Iterator(this, positions_.size()); }

private:
    std::vector<double> positions_;
};

class PortfolioAuditor {
public:
    static double total(const Portfolio& p) {
        double sum = 0.0;
        for (double v : p.positions_) sum += v;
        return sum;
    }
};

int main(){
  Portfolio p;
  p.add(100.0); p.add(250.5); p.add(-30.5);
  for (double v : p) std::cout << v << " ";
  std::cout << "\n";
  std::cout << PortfolioAuditor::total(p) << "\n";
}