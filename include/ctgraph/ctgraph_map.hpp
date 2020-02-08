#pragma once


#include "./ctgraph/ctgraph_std.hpp"
#include "./ctgraph/ctgraph_algorithms.hpp"
#include "./ctgraph/ctgraph_algorithms.hpp"


namespace ctgraph
{

namespace cx
{

template <typename T>
using is_transparent = typename T::is_transparent;
template <typename T, typename = void>
struct has_is_transparent : public std::false_type {};
template <typename T>
struct has_is_transparent<T, std::void_t<is_transparent<T>>> : public std::true_type {};

template <typename Key, typename Value, std::size_t Size = 5,
          typename Compare = std::equal_to<Key>>
class map
{
  public:
    constexpr auto begin() const { return m_data.begin(); }
    constexpr auto begin() { return m_data.begin(); }

    constexpr auto end() const { return m_data.begin() + m_size; }
    constexpr auto end() { return m_data.begin() + m_size; }

    constexpr auto cbegin() const { return m_data.begin(); }
    constexpr auto cend() const { return m_data.begin() + m_size; }

    constexpr auto find(const Key &k)
    {
        return find_impl(*this, k);
    }

    constexpr auto find(const Key &k) const
    {
        return find_impl(*this, k);
    }

    constexpr const Value &at(const Key &k) const
    {
        const auto itr = find(k);
        if (itr != end()) { return itr->second; }
        else { throw std::range_error("Key not found"); }
    }

    template <typename K>
    constexpr const Value &at(const K &k) const
    {
        const auto itr = find(k);
        if (itr != end()) { return itr->second; }
        else { throw std::range_error("Key not found"); }
    }

    constexpr Value &operator[](const Key &k) {
        const auto itr = find(k);
        if (itr == end()) {
            auto &data = m_data[m_size++];
            data.first = k;
            return data.second;
        } else {
            return itr->second;
        }
    }

    template <typename K>
    constexpr Value &operator[](const K &k) {
        const auto itr = find(k);
        if (itr == end()) {
            auto &data = m_data[m_size++];
            data.first = k;
            return data.second;
        } else {
            return itr->second;
        }
    }

    constexpr auto size() const { return m_size; }
    constexpr auto empty() const { return m_size == 0; }

  private:
    template <typename This>
    static constexpr auto find_impl(This &&t, const Key &k)
    {
        return cx::find_if(t.begin(), t.end(),
                           [&k] (const auto &d) { return Compare{}(d.first, k); });
    }

    template <typename This, typename K>
    static constexpr auto find_impl(This &&t, const K &k)
    {
        return cx::find_if(t.begin(), t.end(),
                           [&k] (const auto &d) { return Compare{}(d.first, k); });
    }

    std::array<std::pair<Key, Value>, Size> m_data{};
    std::size_t m_size{0};
};


}

}


