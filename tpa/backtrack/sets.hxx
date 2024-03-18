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

    bt::Result<Value> next() {
        if (set.back() == n) {
            return bt::Backtrack;
        }

        set.back()++;
        for (std::size_t i = 0; i < set.size() - 1; i++) {
            if (set[i] == set.back() || (!ordered && set[i] > set.back())) {
                return bt::Next;
            }
        }

        if (set.size() < k) {
            return bt::Advance;
        }

        return set;
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