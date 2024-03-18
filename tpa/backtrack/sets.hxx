#ifndef TPA_BACKTRACK_SETS_HXX
#define TPA_BACKTRACK_SETS_HXX

#include <vector>

#include "backtrack.hxx"

class Sets {
  public:
    using Value = std::vector<int>;

    Sets(int n, int k, bool ordered) : n(n), k(k), ordered(ordered), set() {
        set.reserve(k);
    }

    bt::Candidate<Value> next() {
        if (set.back() == n) {
            return {};
        }

        set.back()++;
        for (std::size_t i = 0; i < set.size() - 1; i++) {
            if (set[i] == set.back() || (!ordered && set[i] > set.back())) {
                return {.has_next = true};
            }
        }

        if (set.size() < k) {
            return {.has_next = true, .is_partial = true};
        }

        return {.value = set, .has_next = true, .is_partial = true};
    }

    bool advance() {
        if (set.size() == k) {
            return false;
        }
        set.push_back(0);
        return true;
    }

    bool backtrack() {
        if (set.size() > 0) {
            set.pop_back();
        }
        return set.size() != 0;
    }

  private:
    int n, k;
    bool ordered;
    Value set;
};

#endif