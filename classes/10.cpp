#include <iostream>
#include <string>
#include <memory>

class Instrument {
public:
    virtual std::string describe() const {
        return "Instrument";
    }

    virtual ~Instrument() = default;
};

class Bond : public Instrument {
public:
    std::string describe() const override {
        return "Bond";
    }
};

int main(){
  Bond b;
  Instrument sliced = b;
  std::cout << sliced.describe() << "\n";
  Instrument& ref = b;
  std::cout << ref.describe() << "\n";
  std::unique_ptr<Instrument> p = std::make_unique<Bond>();
  std::cout << p->describe() << "\n";
}