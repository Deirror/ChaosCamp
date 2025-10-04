#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <optional>

#include "./mt_bst.hpp"
#include "./st_bst.hpp"

using clk = std::chrono::steady_clock;

template <typename F>
double tms(F&& f) {
    auto t0 = clk::now();
    f();
    auto t1 = clk::now();

    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

int main() {
    const size_t N   = 10000000;
    const size_t KEYR = 500000;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, static_cast<int>(KEYR - 1));

    std::vector<std::pair<int,int>> kv;
    kv.reserve(N);

    for (size_t i = 0; i < N; ++i) {
        kv.emplace_back(dist(rng), static_cast<int>(i));
    }

    std::vector<int> keys;
    keys.reserve(N / 2);

    for (size_t i = 0; i < N / 2; ++i) {
        keys.push_back(dist(rng));
    }

    MultiThreadedBST mt(std::thread::hardware_concurrency());
    SingleThreadedBST st;

    double mtIns = tms([&](){ mt.insert(kv); });
    double mtSch = tms([&](){ std::vector<std::optional<int>> r; mt.search(keys, r); });

    double stIns = tms([&](){ st.insert(kv); });
    double stSch = tms([&](){ std::vector<std::optional<int>> r; st.search(keys, r); });

    std::cout << "MT insert = " << mtIns << " ms, search = " << mtSch << " ms\n";
    std::cout << "ST insert = " << stIns << " ms, search = " << stSch << " ms\n";

    return 0;
}
