#include <iostream>

#include "backtrack.hxx"
#include "sets.hxx"

int main(int argc, char **argv) {
    Sets combinations(std::stoi(argv[1]), std::stoi(argv[2]), false);
    for (const auto &comb : bt::backtrack(std::move(combinations))) {
        for (auto i : comb) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }
}