#include <iostream>

template<typename T>
class MyContainer {
public:
    MyContainer(void) {
        data = T();
    }
    void add(const T& rval) {
        data += rval;
    }
    const T& sum(void) const {
        return data;
    }

private:
    T data;
};

extern template class MyContainer<int>;

template class MyContainer<int>;

int main(){
  MyContainer<int> c;
  c.add(5); c.add(10);
  std::cout << c.sum() << "\n";
}