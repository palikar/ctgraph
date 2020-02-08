#pragma once

#include "./ctgraph/ctgraph_std.hpp"
#include "./ctgraph/ctgraph_algorithms.hpp"
#include "./ctgraph/ctgraph_algorithms.hpp"



namespace ctgraph
{

namespace cx
{

template <typename Value,  Value sentinel, std::size_t Size = 5>
class vector
{
    using storage_t = std::array<Value, Size>;
  public:
    using iterator = typename storage_t::iterator;
    using const_iterator = typename storage_t::const_iterator;
    using value_type = Value;
    using reference = typename storage_t::reference;
    using const_reference = typename storage_t::const_reference;

    template<typename Itr>
    constexpr vector(Itr begin, const Itr &end)
    {
        while (begin != end) {
            push_back(*begin);
            ++begin;
        }
    }
    constexpr vector(std::initializer_list<Value> init)
        : vector(init.begin(), init.end())
    {
    }

    constexpr vector() = default;

    constexpr auto begin() const { return m_data.begin(); }
    constexpr auto begin() { return m_data.begin(); }

    // We would have prefered to use `std::next`, however it does not seem to be
    // enabled for constexpr use for std::array in this version of gcc. As of
    // September 2017 this is fixed in GCC trunk but not in GCC 7.2.
    constexpr auto end() const { return m_data.begin() + m_size; }
    constexpr auto end() { return m_data.begin() + m_size; }

    constexpr auto cbegin() const { return m_data.begin(); }
    constexpr auto cend() const { return m_data.begin() + m_size; }

    constexpr const Value &operator[](const std::size_t t_pos) const {
        return m_data[t_pos];
    }
    constexpr Value &operator[](const std::size_t t_pos) {
        return m_data[t_pos];
    }

    constexpr Value &at(const std::size_t t_pos) {
        if (t_pos >= m_size) {
            // This is allowed in constexpr context, but if the constexpr evaluation
            // hits this exception the compile would fail
            throw std::range_error("Index past end of vector");
        } else {
            return m_data[t_pos];
        }
    }
    constexpr const Value &at(const std::size_t t_pos) const {
        if (t_pos >= m_size) {
            throw std::range_error("Index past end of vector");
        } else {
            return m_data[t_pos];
        }
    }

    constexpr Value& push_back(Value t_v) {
        if (m_size >= Size) {
            throw std::range_error("Index past end of vector");
        } else {
            Value& v = m_data[m_size++];
            v = std::move(t_v);
            return v;
        }
    }

    constexpr Value& pop_front() {
        Value& v = m_data[m_front++];
        return v;
    }
    
    constexpr const Value &back() const {
        if (empty()) {
            throw std::range_error("Index past end of vector");
        } else {
            return m_data[m_size - 1];
        }
    }
    constexpr Value &back() {
        if (empty()) {
            throw std::range_error("Index past end of vector");
        } else {
            return m_data[m_size - 1];
        }
    }

    constexpr auto capacity() const { return Size; }
    constexpr auto size() const { return m_size; }
    constexpr auto empty() const { return m_size == m_front; }

    constexpr void clear() { m_size = 0; }

    constexpr const Value* data() const {
        return m_data.data();
    }

  private:
    storage_t m_data{};
    std::size_t m_size{0};
    std::size_t m_front{0};
};


}



}
