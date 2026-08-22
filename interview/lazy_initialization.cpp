// Make an expensive-to-construct singleton safe to lazily initialize from many threads with
// zero risk of double-construction or a data race—without hand-rolling double-checked locking
// (which is broken pre-C++11 and unnecessary after it). Know both the magic-static idiom and
// std::call_once, since interviewers ask for the second when they want to see the explicit
// mechanism. 

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <cassert>

class ExpensiveConfig {
public:
    static ExpensiveConfig& instance() {
        static ExpensiveConfig instance;
        return instance;
    }
    int value = 0;
private:
    ExpensiveConfig() { value = 42; }
};

int main() {
    const int threads = 16;
    std::vector<ExpensiveConfig*> results(threads);
    std::vector<std::thread> pool;

    for (int t = 0; t < threads; ++t)
        pool.emplace_back([&, t] { results[t] = &ExpensiveConfig::instance(); });
    for (auto& th : pool) th.join();

    for (int t = 1; t < threads; ++t)
        assert(results[t] == results[0]); // every thread gets the same instance
    assert(results[0]->value == 42);

    std::cout << "PASS\n";
    return 0;
}