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

        using iterator_category = std::bidirectional_iterator_tag;
        using pointer = value_type *;
        using reference = value_type &;

        fixed_vec *vec{ nullptr };
        index_type index{ 0 };

        iter() = default;
        iter(fixed_vec &vec) : vec(&vec) {}
        iter(fixed_vec &vec, index_type index) : vec(&vec), index(index) {}

        reference operator*() const { return vec->values_.at(index); }
        reference operator*() { return vec->values_.at(index); }

        iter &operator++()
        {
            index++;
            return *this;
        }

        iter operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        iter &operator--()
        {
            index--;
            return *this;
        }

        iter operator--(int)
        {
            auto tmp = *this;
            --*this;
            return tmp;
        }

        bool operator==(const iter &other) const { return index == other.index && vec == other.vec; }
        bool operator!=(const iter &other) const { return index != other.index || vec != other.vec; }
    };

    auto begin() -> iter { return iter(*this); }
    auto end() -> iter { return iter(*this, size_); }
    auto size() const -> std::size_t { return size_; }

    auto push_back(dT val)
    {
        if (size() < Cap) {
            values_.at(size_) = val;
            size_ += 1;
        }
    }

    auto at(const index_type index) -> dT & { return values_.at(index); }
};

}// namespace detail
