// Two threads transferring funds in opposite directions (A→B and B→A) will deadlock if each
// locks its 'from' account before its 'to' account—classic lock-ordering inversion. Fix it with
// std::scoped_lock, which locks an arbitrary number of mutexes together using a
// deadlock-avoidance algorithm. (Pre-C++17: the equivalent is std::lock() plus two
// std::lock_guards constructed with std::adopt_lock.) 

#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <cassert>

struct Account {
    std::mutex mtx;
    long balance;
};

// TODO: transfer funds without risking deadlock, even when threads
// transfer in opposite directions (A->B and B->A) concurrently.
void transfer(Account& from, Account& to, long amount) {

    if (&from.mtx < &to.mtx) {
        std::unique_lock lock_from(from.mtx);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::unique_lock lock_to(to.mtx);
    } else {
        std::unique_lock lock_to(to.mtx);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::unique_lock lock_from(from.mtx);
    }
    // std::unique_lock lock_from(from.mtx);
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // std::unique_lock lock_to(to.mtx);

    from.balance -= amount;
    to.balance += amount;

    return;
}

int main() {
    Account a{{}, 1000};
    Account b{{}, 1000};

    std::vector<std::thread> threads;
    for (int i = 0; i < 20; ++i) {
        std::cout << i << std::endl;
        threads.emplace_back(transfer, std::ref(a), std::ref(b), 1);
        threads.emplace_back(transfer, std::ref(b), std::ref(a), 1);
    }
    for (auto& t : threads) t.join();

    assert(a.balance + b.balance == 2000); // nothing lost or duplicated
    std::cout << "PASS: a=" << a.balance << " b=" << b.balance << "\n";
    return 0;
}