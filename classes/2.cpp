#include <cstring>
#include <iostream>

class Buffer
{
    private:
    size_t count;
    int* data;

    public:
    Buffer(size_t count) : count(count) {
        data = (int*)malloc(sizeof(data) * count);
    }

    ~Buffer() {
        free(data);
    }

    Buffer(const Buffer& other) {
        count = other.count;
        data = new int[count];
        memcpy(data, other.data, other.count);
    }

    Buffer& operator=(const Buffer& other) {
        if (this == &other) {
            return *this;
        }

        count = other.count;
        free(data);
        data = new int[count];
        memcpy(data, other.data, count);

        return *this;
    }

    void set(size_t pos, int val) {
        data[pos] = val;
    }

    int get(size_t pos) {
        return data[pos];
    }
};

int main(){
  Buffer a(3);
  a.set(0,1); a.set(1,2); a.set(2,3);
  Buffer b(a);
  b.set(0,99);
  std::cout << a.get(0) << " " << b.get(0) << "\n";
  Buffer c(1);
  c = a;
  c.set(1,42);
  std::cout << c.get(0) << " " << c.get(1) << " " << a.get(1) << "\n";
  c = c;
  std::cout << c.get(1) << "\n";
}