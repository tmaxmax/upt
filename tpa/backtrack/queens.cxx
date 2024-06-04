#include <algorithm>
#include <iostream>
#include <span>
#include <vector>

#include "backtrack.hxx"

class Queens {
  public:
    using Value = std::span<const int>;

    Queens(int n) : n(n) { queens.reserve(n); }

    bt::Result<Value> next() {
        if (queens.back() == n) {
            return bt::Backtrack;
        }

        queens.back()++;
        for (std::size_t i = 0; i < queens.size() - 1; i++) {
            if (queens[i] == queens.back()) {
                return bt::Next;
            }
            const auto col_dist = queens.size() - i - 1;
            const auto row_dist = queens.back() < queens[i]
                                      ? queens[i] - queens.back()
                                      : queens.back() - queens[i];
            if (col_dist == row_dist) {
                return bt::Next;
            }
        }

        if (queens.size() < n) {
            return bt::Advance;
        }

        return queens;
    }

    bool advance() {
        if (queens.size() == n) {
            return false;
        }
        queens.push_back(0);
        return true;
    }

    bool backtrack() {
        if (queens.size() > 0) {
            queens.pop_back();
        }
        return queens.size() != 0;
    }

  private:
    int n;
    std::vector<int> queens;
};

int main(int argc, char **argv) {
    const auto n = std::stoi(argv[1]);
    for (const auto &positioning : bt::backtrack(Queens(n))) {
        std::vector<std::pair<int, int>> coords;
        for (int i = 0; i < positioning.size(); i++) {
            coords.emplace_back(positioning[i], i);
        }
        std::sort(coords.begin(), coords.end(),
                  [](auto a, auto b) { return a.first < b.first; });
        for (auto c : coords) {
            for (int i = 0; i < n; i++) {
                std::cout << (i == c.second ? 'Q' : '.') << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
}