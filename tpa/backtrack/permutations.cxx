#include <iostream>

#include "backtrack.hxx"
#include "sets.hxx"

int main(int argc, char **argv) {
    const auto n = std::stoi(argv[1]);
    Sets permutations(n, n, true);

    for (const auto &perm : bt::backtrack(std::move(permutations))) {
        for (auto i : perm) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }
}
