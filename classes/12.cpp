#include <iostream>
#include <string>

class Animal {
public:
    explicit Animal(std::string name) : name(std::move(name)) { };

    virtual void speak() const {
        std::cout << name << " makes a sound\n";
    }

    virtual ~Animal() = default;
    std::string name;
};

// If Swimmer and Walker inherited from Animal non-virtually, the Duck object
// would have two instances of Animal which would be ambiguous at compile time
// when accessing speak() or name.
class Swimmer : public virtual Animal {
public:
    explicit Swimmer(const std::string& name): Animal(name) { };

    void swim() const {
        std::cout << name << " swims\n";
    }
};

class Walker : public virtual Animal {
public:
    explicit Walker(const std::string& name): Animal(name) { };

    void walk() const {
        std::cout << name << " walks\n";
    }
};

class Duck : public Swimmer, public Walker {
public:
    explicit Duck(const std::string& name): Animal(name), Swimmer(name), Walker(name) { };
};

int main(){
  Duck d("Donald");
  d.swim();
  d.walk();
  d.speak();
  std::cout << d.name << "\n";
}