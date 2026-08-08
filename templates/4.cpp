#include <iostream>

template<typename... T>
constexpr size_t count(void) {
   return sizeof...(T);
}

template<typename... Ts>
struct Tuple {};

template<typename Head, typename... Tail>
struct Tuple<Head, Tail...> : Tuple <Tail...> {
    Tuple(Head h, Tail... t) : Tuple<Tail...>(t...), head(h) {}

    Head head;
};

template<size_t N, typename... Ts>
struct GetHelper;

template<typename Head, typename... Tail>
struct GetHelper<0, Head, Tail...> {
    static Head& get(Tuple<Head, Tail...>& t) { return t.head; }
};

template<size_t N, typename Head, typename... Tail>
struct GetHelper<N, Head, Tail...> {
    static auto& get(Tuple<Head, Tail...>& t) {
        Tuple<Tail...>& base = t; // slice to the base sub-tuple
        return GetHelper<N - 1, Tail...>::get(base);
    }
};

template<size_t N, typename... Ts>
auto& get(Tuple<Ts...>& t) {
    return GetHelper<N, Ts...>::get(t);
}

int main(){
  std::cout << count<int,double,char>() << "\n";
  Tuple<int,double,std::string> t{1, 2.5, std::string("hi")};
  std::cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << "\n";
}