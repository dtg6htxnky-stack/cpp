#include <cstddef>
#include <initializer_list>
#include <iostream>

template<typename T, size_t SIZE>
class FixedVector {
public:
    FixedVector() = default;
    FixedVector(std::initializer_list<T> list) {
        int index = 0;
        for (T e : list) {
            buffer_[index++] = e;
        }
    }

    const T& operator[](size_t index) const {
        return buffer_[index];
    }

    T& operator[](size_t index) {
        return buffer_[index];
    }

    template<typename Y>
    FixedVector<Y, SIZE> cast(void) const {
        FixedVector<Y, SIZE> result;
        for (size_t i = 0; i < SIZE; ++i) {
            result[i] = static_cast<Y>(buffer_[i]);
        }
        return result;
    }

private:
    T buffer_[SIZE];
};

int main(){
  FixedVector<int,3> v{1,2,3};
  v[1] = 20;
  std::cout << v[0] << " " << v[1] << " " << v[2] << "\n";
  auto d = v.cast<double>();
  std::cout << d[0] << " " << d[1] << " " << d[2] << "\n";
}