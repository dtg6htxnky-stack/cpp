#include <iostream>
#include <vector>

class Instrumented
{
public:
    explicit Instrumented(int value) : value(value) { }

    Instrumented(const Instrumented& other) : value(other.value) {
        copies += 1;
    }

    Instrumented(const Instrumented&& other) noexcept : value(other.value) {
        moves += 1;
    }

    Instrumented& operator=(const Instrumented&) = default;
    Instrumented& operator=(Instrumented&&) noexcept = default;

    static int copies;
    static int moves;

private:
    int value;
};

int Instrumented::copies = 0;
int Instrumented::moves = 0;

int main(){
  {
    std::vector<Instrumented> v;
    v.reserve(3);
    v.push_back(Instrumented(1));
    v.push_back(Instrumented(2));
    v.push_back(Instrumented(3));
    std::cout << "reserved: copies=" << Instrumented::copies << " moves=" << Instrumented::moves << "\n";
  }
  Instrumented::copies = 0; Instrumented::moves = 0;
  {
    std::vector<Instrumented> v;
    v.push_back(Instrumented(1));
    v.push_back(Instrumented(2));
    v.push_back(Instrumented(3));
    std::cout << "unreserved: copies=" << Instrumented::copies << " moves=" << Instrumented::moves << "\n";
  }
}