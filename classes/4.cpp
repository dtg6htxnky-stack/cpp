#include <cstddef>
#include <iostream>
#include <vector>
#include <numeric>

class Series
{
    private:
    std::vector<double> data;
    mutable double cacheSum;
    mutable bool cacheIsValid = false;

    public:
    Series(std::initializer_list<double> values) : data(values) { }
    
    double sum() const
    {
        if (cacheIsValid) {
            return cacheSum;
        }

        cacheSum = 0;
        for (int i = 0; i < data.size(); ++i) {
            cacheSum += data[i];
        }
        cacheIsValid = true;
        return cacheSum;
    }

    double& operator[](size_t index)
    {
        cacheIsValid = false;
        return data[index];
    }

    double operator[](size_t index) const
    {
        return data[index];
    }
};

void printFirst(const Series& s) {
  std::cout << s[0] << "\n";
}
int main(){
  Series s{1.0, 2.0, 3.0};
  std::cout << s.sum() << "\n";
  std::cout << s.sum() << "\n";
  s[0] = 100.0;
  std::cout << s.sum() << "\n";
  printFirst(s);
}