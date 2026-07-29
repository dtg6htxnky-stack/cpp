#include <iostream>
#include <vector>

class ScopedHandle
{
public:
    explicit ScopedHandle(int id): id(id) { };
    ScopedHandle(ScopedHandle& other) = delete;
    ScopedHandle& operator=(ScopedHandle& other) = delete;

    ScopedHandle(ScopedHandle&& other): id(other.id)
    {
        other.id = -1;
    }

    ScopedHandle& operator=(ScopedHandle&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        id = other.id;
        other.id = -1;
        return *this;
    }

    ~ScopedHandle()
    {
        if (id >= 0) {
            std::cout << "closing " << id << "\n";
        }
    }

    int get()
    {
        return id;
    }

private:
    int id;
};

int main(){
  ScopedHandle a(5);
  ScopedHandle b(std::move(a));
  std::cout << a.get() << " " << b.get() << "\n";
  std::vector<ScopedHandle> handles;
  handles.push_back(ScopedHandle(10));
  handles.push_back(std::move(b));
  std::cout << handles[0].get() << " " << handles[1].get() << "\n";
}