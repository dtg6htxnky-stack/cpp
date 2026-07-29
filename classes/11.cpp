#include <iostream>

class BadBase {
public:
    ~BadBase() {
        std::cout << "~BadBase\n";
    }
};

class BadDerived : public BadBase {
public:
    ~BadDerived() {
        std::cout << "~BadDerived\n";
    }
};

class GoodBase {
public:
    virtual ~GoodBase() {
        std::cout << "~GoodBase\n";
    }
};

class GoodDerived : public GoodBase {
public:
    ~GoodDerived() {
        std::cout << "~GoodDerived\n";
    }
};

int main(){
  BadBase* bad = new BadDerived();
  delete bad;
  std::cout << "---\n";
  GoodBase* good = new GoodDerived();
  delete good;
}