#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <ranges>
#include <span>
#include <vector>

#include "backtrack.hxx"

namespace vw = std::ranges::views;
namespace rg = std::ranges;

struct Object {
    int profit, weight;
};

class Backpack {
  public:
    using Value = int;

    Backpack(std::span<const Object> objs, int max_weight)
        : objects{objs}, max_weight{max_weight}, max_found_profit{} {}

    bt::Result<Value> next() {
        if (object_indexes.back() == objects.size()) {
            if (is_candidate) {
                is_candidate = false;
                return max_found_profit;
            }

            return bt::Backtrack;
        }

        object_indexes.back()++;

        const auto new_weight = weight();
        if (new_weight > max_weight) {
            if (is_candidate) {
                is_candidate = false;
                return max_found_profit;
            }

            return bt::Next;
        }

        const auto new_profit = profit();
        if (new_profit >= max_found_profit) {
            max_found_profit = new_profit;
            is_candidate = true;
        }

        return bt::Advance;
    }

    bool advance() {
        if (object_indexes.size() == objects.size()) {
            return false;
        }
        if (object_indexes.size() == 0) {
            object_indexes.push_back(-1);
        } else {
            object_indexes.push_back(object_indexes.back());
        }
        return true;
    }

    bool backtrack() {
        if (object_indexes.size() > 0) {
            object_indexes.pop_back();
        }
        return object_indexes.size() != 0;
    }

  private:
    int weight() const {
        auto v = object_indexes |
                 vw::transform([&](int i) { return objects[i].weight; });

        return std::accumulate(rg::begin(v), rg::end(v), 0);
    }

    int profit() const {
        auto v = object_indexes |
                 vw::transform([&](int i) { return objects[i].profit; });

        return std::accumulate(rg::begin(v), rg::end(v), 0);
    }

    int max_weight;
    int max_found_profit;
    bool is_candidate;
    std::vector<int> object_indexes;
    std::span<const Object> objects;
};

extern "C" int solve_backpack(const int *weights, const int *profits,
                              const int n, const int max_weight) {
    auto objs = vw::iota(0, n) | vw::transform([&](auto i) -> Object {
                    return {profits[i], weights[i]};
                });
    std::vector<Object> obj_vec{objs.begin(), objs.end()};

    bt::backtrack backpack(Backpack(obj_vec, max_weight));

    return std::accumulate(backpack.begin(), backpack.end(), 0,
                           [](auto a, auto b) { return std::max(a, b); });
}