#pragma once

#include <array>

namespace detail {

// used as a simple wrapper for fixed size elements
template<typename T, std::size_t Cap>
struct fixed_vec
{
    using dT = std::remove_cv_t<T>;
    using index_type = std::size_t;

    std::array<dT, Cap> values_ = {};
    std::size_t size_{ 0 };

    struct iter
    {
        using difference_type = std::ptrdiff_t;
        using value_type = dT;

        index_type index{ 0 };
        fixed_vec &vec;

        iter(fixed_vec &vec) : vec(vec) {}
        iter(fixed_vec &vec, index_type index) : vec(vec), index(index) {}

        value_type operator*() const { return vec.values_.at(index); }

        iter &operator++() { index++; }

        iter operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        iter &operator--() { index--; }

        iter operator--(int)
        {
            auto tmp = *this;
            --*this;
            return tmp;
        }

        bool operator==(const iter &other) const { return index == other.index; }
    };

    auto begin() -> iter { return iter(*this); }
    auto end() -> iter { return iter(*this, size_); }
    auto size() const -> std::size_t { return size_; }

    auto push_back(dT val)
    {
        if (size() < Cap) {
            size_ += 1;
            values_.at(size_) = val;
        }
    }
};

}// namespace detail
