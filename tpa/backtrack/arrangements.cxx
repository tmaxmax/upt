#include <iostream>

#include "backtrack.hxx"
#include "sets.hxx"

int main(int argc, char **argv) {
    Sets arrangements(std::stoi(argv[1]), std::stoi(argv[2]), true);
    for (const auto &arrn : bt::backtrack(std::move(arrangements))) {
        for (auto i : arrn) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }
}