#include <cstddef>
#include <initializer_list>
#include <iostream>

template<typename T, size_t ARRAY_SIZE>
class FixedVector 
{
    private:
    T values[ARRAY_SIZE];

    public:

    T& operator[](size_t index)
    {
        return values[index];
    }

    const T& operator[](size_t index) const
    {
        return values[index];
    }

    template<typename U>
    FixedVector<U, ARRAY_SIZE> cast() const
    {
        return FixedVector<U, ARRAY_SIZE>{values};
    }
};

int main(){
  FixedVector<int,3> v{1,2,3};
  v[1] = 20;
  std::cout << v[0] << " " << v[1] << " " << v[2] << "\n";
  auto d = v.cast<double>();
  std::cout << d[0] << " " << d[1] << " " << d[2] << "\n";
}