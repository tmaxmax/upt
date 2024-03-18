#include <iostream>
#include <thread>

#include "backtrack.hxx"
#include "sets.hxx"

int main(int argc, char **argv) {
    const auto n = std::stoi(argv[1]);
    const auto k = std::stoi(argv[2]);

    std::size_t idx_solution = 0;
    for (const auto &arrn : bt::backtrack(Sets(n, k, true))) {
        if (idx_solution > 0) {
            if (idx_solution % k == 0) {
                for (int i = 0; i < k; i++) {
                    std::cout << "\033[F";
                }
            }
            std::cout << '\n';
        }
        idx_solution++;
        for (auto i : arrn) {
            std::cout << i << ' ';
            std::this_thread::sleep_for(std::chrono::microseconds{300});
        }
    }

    std::cout << '\n';
}