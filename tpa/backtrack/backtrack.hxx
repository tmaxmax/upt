#ifndef TPA_BACKTRACK_H
#define TPA_BACKTRACK_H

#include <concepts>
#include <iterator>
#include <optional>
#include <type_traits>
#include <variant>

namespace bt {
enum class Flow { Backtrack, Next, Advance };
using enum Flow;

template <typename T> using Result = std::variant<T, Flow>;

template <typename S>
concept State = std::movable<S> &&
                requires(S s) {
                    typename S::Value;
                    {
                        s.next()
                        } -> std::convertible_to<Result<typename S::Value>>;
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
      public:
        using iterator_category = std::input_iterator_tag;
        using value_type = typename S::Value;
        using reference = value_type &;
        using pointer = value_type *;
        using difference_type = std::ptrdiff_t;

        reference operator*() const { return last_value.value(); }
        pointer operator->() const { return &last_value.value(); }

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

      private:
        std::optional<std::reference_wrapper<S>> state;
        std::optional<value_type> last_value;

        friend class backtrack;

        struct result_handler {
            iterator &it;
            bool was_value;

            bool operator()(value_type v) noexcept {
                it.last_value = v;
                was_value = true;
                return false;
            }
            bool operator()(Flow f) noexcept {
                switch (f) {
                case Advance:
                    if (it.state->get().advance()) {
                        return false;
                    }
                case Backtrack:
                    if (!it.state->get().backtrack()) {
                        it.state = std::nullopt;
                    }
                    return false;
                case Next:
                    return true;
                }
            }
        };

        void next() {
            while (state) {
                result_handler handler{*this};
                while (std::visit(handler, state->get().next()))
                    ;
                if (handler.was_value) {
                    break;
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
    };

    static_assert(std::input_iterator<iterator>);
    static_assert(std::sentinel_for<sentinel, iterator>);

    iterator begin() { return iterator{state}; }

    sentinel end() { return sentinel{}; }
};

} // namespace bt

#endif // TPA_BACKTRACK_H
