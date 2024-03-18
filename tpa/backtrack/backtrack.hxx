#ifndef TPA_BACKTRACK_H
#define TPA_BACKTRACK_H

#include <concepts>
#include <iterator>
#include <optional>
#include <type_traits>

namespace bt {
template <typename T> struct Candidate {
    std::optional<std::reference_wrapper<T>> value;
    bool has_next, is_partial;
};

template <typename S>
concept State =
    std::movable<S> && requires(S s) {
                           typename S::Value;
                           {
                               s.next()
                               } -> std::same_as<Candidate<typename S::Value>>;
                           { s.advance() } -> std::same_as<bool>;
                           { s.backtrack() } -> std::same_as<bool>;
                       };

template <State S> class backtrack {
  private:
    S state;

  public:
    backtrack(S s) : state(std::move(s)) {}

    class sentinel {};

    class iterator {
      private:
        std::optional<std::reference_wrapper<S>> state;
        std::optional<std::reference_wrapper<typename S::Value>> last_value;

        friend class backtrack;

        void next() {
            while (state) {
                auto &s = state->get();
                auto c = s.next();
                for (; c.has_next && !c.is_partial; c = s.next())
                    ;
                if (c.has_next) {
                    if (c.value) {
                        last_value = c.value;
                        break;
                    } else if (s.advance()) {
                        continue;
                    }
                }
                if (!s.backtrack()) {
                    state = std::nullopt;
                }
            }
        }

        iterator(S &s) : state(s) {
            if (state->get().advance()) {
                next();
            } else {
                state = std::nullopt;
            }
        }

      public:
        using iterator_category = std::input_iterator_tag;
        using value_type = typename S::Value;
        using reference = value_type &;
        using pointer = value_type *;
        using difference_type = std::ptrdiff_t;

        reference operator*() const { return last_value->get(); }
        pointer operator->() const { return &last_value->get(); }

        iterator &operator++() {
            next();
            return *this;
        }

        iterator operator++(int) {
            auto copy = *this;
            ++*this;
            return copy;
        }

        bool operator==(const sentinel &other) const {
            return !state.has_value();
        }

        bool operator!=(const sentinel &other) const {
            return !(*this == other);
        }
    };

    static_assert(std::input_iterator<iterator>);
    static_assert(std::sentinel_for<sentinel, iterator>);

    iterator begin() { return iterator{state}; }

    sentinel end() { return sentinel{}; }
};

} // namespace bt

#endif // TPA_BACKTRACK_H
