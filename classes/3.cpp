#include <cstring>
#include <iostream>
#include <ostream>

class Matrix
{
    private:
    size_t count;
    int* data;

    public:
    Matrix(size_t n): count(n) 
    {
        if (n > 0) {
            data = new int[n];
        } else {
            data = NULL;
        }
    }

    Matrix(const Matrix& other): count(other.count) {
        data = new int[other.count];
        memcpy(data, other.data, other.count);
    }

    Matrix(Matrix&& other): count(other.count) {
        data = other.data;
        other.data = NULL;
        other.count = 0;
    }

    Matrix& operator=(const Matrix& other)
    {
        if (this == &other) {
            return *this;
        }

        count = other.count;
        delete[] data;
        data = new int[other.count];
        memcpy(data, other.data, other.count);

        return *this;
    }

    Matrix& operator=(Matrix&& other)
    {
        if (this == &other) {
            return *this;
        }

        data = other.data;
        other.data = NULL;
        count = other.count;
        other.count = 0;

        return *this;
    }

    ~Matrix()
    {
        delete[] data;
    }

    void set(size_t index, int value)
    {
        data[index] = value;
    }

    int get(size_t index)
    {
        return data[index];
    }

    size_t size()
    {
        return count;
    }

    bool empty()
    {
        return this->size() == 0;
    }
};

int main(){
  Matrix a(3);
  a.set(0,10); a.set(1,20); a.set(2,30);
  Matrix b(std::move(a));
  std::cout << b.get(0) << " " << b.get(1) << " " << b.get(2) << "\n";
  std::cout << std::boolalpha << a.empty() << " " << a.size() << "\n";
  Matrix c(1);
  c = std::move(b);
  std::cout << c.get(2) << " " << std::boolalpha << b.empty() << "\n";
}