#include <iostream>
#include <string>
#include <vector>

template <typename T>
class TypeName {
public:
    static const char* get(void) {
        return "unknown";
    }
};

template <>
class TypeName<int> {
public:
    static std::string get(void) {
        return "int";
    }
};

template <>
class TypeName<double> {
public:
    static std::string get(void) {
        return "double";
    }
};

template <>
class TypeName<bool> {
public:
    static std::string get(void) {
        return "bool";
    }
};

template <>
class TypeName<std::string> {
public:
    static std::string get(void) {
        return "string";
    }
};

template <typename T>
class TypeName<T*> {
public:
    static std::string get(void) {
        return std::string(TypeName<T>::get()) + " pointer";
    }
};

template <typename T>
class TypeName<std::vector<T>> {
public:
    static std::string get(void) {
        return std::string(TypeName<T>::get()) + " vector";
    }
};

int main(){
  std::cout << TypeName<int>::get() << "\n";
  std::cout << TypeName<double>::get() << "\n";
  std::cout << TypeName<bool>::get() << "\n";
  std::cout << TypeName<std::string>::get() << "\n";
  std::cout << TypeName<int*>::get() << "\n";
  std::cout << TypeName<std::vector<int>>::get() << "\n";
}