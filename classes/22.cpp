#include <cstddef>
#include <iostream>
#include <cstring>

class SmallString {
public:
    SmallString(const char* str) {
        size_ = strlen(str);
        if (size_ < sizeof(bufferShort_)) {
            memcpy(&bufferShort_, str, size_ + 1);
            bufferLong_ = NULL;
        } else {
            bufferLong_ = new char[size_ + 1];
            memcpy(bufferLong_, str, size_ + 1);
        }
    }

    ~SmallString() { delete[] bufferLong_; }

    bool isSmall(void) const {
        return (bufferLong_ == NULL);
    }

    const char* data(void) const {
        if (isSmall()) {
            return bufferShort_;
        }

        return bufferLong_;
    }

    size_t size(void) const {
        return size_;
    }

private:
    char* bufferLong_;
    char bufferShort_[16];
    size_t size_;
};

int main(){
  SmallString ticker("AAPL");
  SmallString longName("this-is-a-very-long-symbol-name-exceeding-inline-cap");
  std::cout << std::boolalpha << ticker.isSmall() << " " << longName.isSmall() << "\n";
  std::cout << ticker.data() << "\n";
  std::cout << longName.data() << "\n";
  std::cout << ticker.size() << " " << longName.size() << "\n";
  std::cout << sizeof(SmallString) << "\n";
}