#pragma once

#include "./ctgraph/ctgraph_std.hpp"
#include "./ctgraph/ctgraph_iterators.hpp"
#include "./ctgraph/ctgraph_algorithms.hpp"
#include "./ctgraph/ctgraph_map.hpp"
#include "./ctgraph/ctgraph_vector.hpp"


namespace ctgraph
{

namespace detail
{

template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T &value)
{
    for (; first != last; ++first)
    {
        if (*first == value)
        {
            return first;
        }
    }
    return last;
}

template<size_t Index, size_t Count, typename EnumType, typename Nodes>
static constexpr size_t count_(const EnumType val, Nodes &&nodes)
{

    auto &node = std::get<Index>(std::forward<Nodes>(nodes));
    if (node.m_name == val)
    {
        return node.m_tos.size();
    }
    else
    {
        if constexpr (Index + 1 < Count)
        {
            return count_<Index + 1, Count>(val, std::forward<Nodes>(nodes));
        }
        else
        {
            return 0;
        }
    }
}

template<size_t N, size_t Index, size_t Count, typename EnumType, typename Nodes>
constexpr static EnumType get_node_impl(const EnumType val, Nodes &&nodes)
{
    auto &node = std::get<Index>(std::forward<Nodes>(nodes));
    if (node.m_name == val)
    {
        if (N < node.m_tos.size())
        {
            return (node.m_tos)[N];
        }
        else
        {
            return val;
        }
    }
    else
    {
        if constexpr (Index + 1 < Count)
        {
            return get_node_impl<N, Index + 1, Count>(val, std::forward<Nodes>(nodes));
        }
        else
        {
            return val;
        }
    }
}

template<size_t Index, size_t Count, typename EnumType, typename Nodes, typename Callable>
static constexpr void for_each_(const EnumType val, Nodes &&nodes, Callable &&call)
{
    auto &node = std::get<Index>(std::forward<Nodes>(nodes));
    if (node.m_name == val)
    {
        std::for_each(std::begin(node.m_tos), std::end(node.m_tos), std::forward<Callable>(call));
    }
    else
    {
        if constexpr (Index + 1 < Count)
        {
            for_each_<Index + 1, Count>(val, std::forward<Nodes>(nodes), std::forward<Callable>(call));
        }
    }
}

template<size_t Index, size_t Count, typename EnumType, typename Nodes, typename Callable>
static constexpr auto apply_(const EnumType val, Nodes &&nodes, Callable &&call)
{
    const auto &node = std::get<Index>(std::forward<Nodes>(nodes));
    if (node.m_name == val)
    {
        return call(node.m_tos);
    }
    else
    {
        if constexpr (Index + 1 < Count)
        {
            return apply_<Index + 1, Count>(val, std::forward<Nodes>(nodes), std::forward<Callable>(call));
        }
        else
        {
            const std::array<EnumType, 1> arr{ static_cast<EnumType>(0) };
            return call(arr);
        }
    }
}

template<size_t Index, size_t Count, typename EnumType, typename Nodes>
static constexpr auto contains_(const EnumType val, Nodes &&nodes)
{
    const auto &node = std::get<Index>(std::forward<Nodes>(nodes));
    if (node.m_name == val)
    {
        return true;
    }
    else
    {
        if constexpr (Index + 1 < Count)
        {
            return contains_<Index + 1, Count>(val, std::forward<Nodes>(nodes));
        }
        else
        {
            return false;
        }
    }
}


template<size_t Index, size_t Count, typename EnumType, typename Nodes>
static constexpr auto adjacent_(const EnumType val, const EnumType to, Nodes &&nodes)
{
    auto &node = std::get<Index>(std::forward<Nodes>(nodes));
    if (node.m_name == val)
    {
        return find(std::begin(node.m_tos), std::end(node.m_tos), to) != std::end(node.m_tos);
    }
    else
    {
        if constexpr (Index + 1 < Count)
        {
            return adjacent_<Index + 1, Count>(val, to, std::forward<Nodes>(nodes));
        }
        else
        {
            return false;
        }
    }
}

template<typename Nodes, size_t... Is>
static constexpr auto vert_(Nodes &&nodes, std::index_sequence<Is...>)
{
    // return std::array<typename decltype(std::get<0>(nodes))::value_type, std::tuple_size<Nodes>::value>((std::get<Is>(nodes).m_name) ...);
    return std::array{ (std::get<Is>(nodes).m_name)... };
}

template<typename EnumType, typename Nodes, typename... NodeType,
         size_t... Is, std::size_t MapSize>
constexpr bool path_(EnumType from, EnumType to, Nodes nodes, cx::map<EnumType, bool, MapSize> visited, std::index_sequence<Is...>)
{
    std::array arr{ (std::tuple(std::get<Is>(nodes).m_name, std::get<Is>(nodes).m_cnt, std::get<Is>(nodes).m_tos.data()))... };
    for (const auto [node, cnt, tos] : arr)
    {

        if (node != from)
        {
            continue;
        }

        if (cnt == 0)
        {
            return false;
        }
        
        if (detail::adjacent_<0, sizeof...(NodeType)>(from, to, nodes))
        {
            return true;
        }
        else
        {
            visited[node] = true;
            for (size_t i = 0; i < cnt; ++i) {
                if (visited[*(tos+i)]) {
                    continue;
                }
                if (path_<EnumType, decltype(nodes), NodeType...>(
                        *(tos+i), to, nodes, visited, std::make_index_sequence<sizeof...(Is)>())) {
                    return true;
                }
            }
        }
    }
    return false;
}

template<typename EnumType, typename Nodes, typename... NodeType,
         size_t... Is>
constexpr auto fill_visited_(Nodes nodes, std::index_sequence<Is...>)
{
    cx::map<EnumType, bool, sizeof...(Is)> visited{};
    std::array arr{ (std::tuple(std::get<Is>(nodes).m_name, std::get<Is>(nodes).m_cnt, std::get<Is>(nodes).m_tos.data()))... };
    for (const auto [node, cnt, tos] : arr)
    {
        visited[node] = false;
    }
    return visited;
}

template<typename EnumType, typename Nodes, bool call_after = false, typename... NodeType,
         size_t... Is, std::size_t MapSize, typename Callable>
constexpr void dfs_(EnumType from, Callable && call, Nodes nodes, cx::map<EnumType, bool, MapSize> &visited, std::index_sequence<Is...>)
{
    if (visited[from]) { return; }
    std::array arr{ (std::tuple(std::get<Is>(nodes).m_name, std::get<Is>(nodes).m_cnt, std::get<Is>(nodes).m_tos.data()))... };
    for (const auto [node, cnt, tos] : arr)
    {
        if (node != from) { continue; }
        
        visited[node] = true;

        if constexpr (!call_after) { call(node); }
        
        for (size_t i = 0; i < cnt; ++i) {
            if (visited[*(tos+i)]) { continue; }

            dfs_<EnumType, decltype(nodes), call_after, NodeType...>(*(tos+i), call, nodes, visited,
                                                                     std::make_index_sequence<sizeof...(Is)>());
        }

        if constexpr (call_after) { call(node); }
        
        
        
    }
}

template<typename Nodes, size_t... Is>
static constexpr auto sinks_(Nodes &&nodes, std::index_sequence<Is...>)
{
    return ((std::get<Is>(nodes).m_cnt == 0 ? 1 : 0) + ... );
}

template<typename EnumType, typename Nodes, size_t... Is>
static constexpr auto sources_(Nodes &&nodes, std::index_sequence<Is...>)
{ 
    const std::array arr{ (std::tuple(std::get<Is>(nodes).m_name, std::get<Is>(nodes).m_cnt, std::get<Is>(nodes).m_tos.data()))... };

    cx::map<EnumType, int, sizeof...(Is)>ins{};
    [[maybe_unused]] auto s = ((ins[std::get<Is>(nodes).m_name] = 0 ? 1 : 0) + ... );
    
    for (const auto [node_1, cnt_1, tos_1] : arr)
    {
        for (const auto [node_2, cnt_2, tos_2] : arr)
        {
            if (detail::adjacent_<0, sizeof...(Is)>(node_1, node_2, nodes)) {
                ins[node_2]++;
            }
            
        }
    }
    
    return ((ins[std::get<Is>(nodes).m_name] == 0 ? 1 : 0) + ...);
}

template<typename EnumType, typename Nodes, typename... NodeType, size_t... Is>
constexpr auto source_nodes(Nodes nodes, std::index_sequence<Is...>)
{
    const std::array arr{ (std::tuple(std::get<Is>(nodes).m_name, std::get<Is>(nodes).m_cnt, std::get<Is>(nodes).m_tos.data()))... };
    cx::map<EnumType, int, sizeof...(Is)>ins{};
    
    for (const auto [node, cnt, tos] : arr)
    {
        ins[node] = 0;
    }
    
    for (const auto [node_1, cnt_1, tos_1] : arr)
    {
        for (const auto [node_2, cnt_2, tos_2] : arr)
        {
            if (detail::adjacent_<0, sizeof...(Is)>(node_1, node_2, nodes)) {
                ins[node_2]++;
            }
            
        }
    }

    std::array<EnumType, sizeof...(Is)> sources{};
    int index = 0;
    ((ins[std::get<Is>(nodes).m_name] == 0 ? (sources[index++] = std::get<Is>(nodes).m_name) : static_cast<EnumType>(0)),  ...);
    return sources;
}

template<typename Nodes, size_t... Is, typename EnumType, size_t SinksSize>
static constexpr auto fill_sinks_(Nodes &&nodes,std::array<EnumType, SinksSize>& sinks_arr, std::index_sequence<Is...>)
{

    int i = 0;
    ((std::get<Is>(nodes).m_cnt == 0 ? (sinks_arr[i++] = std::get<Is>(nodes).m_name) : static_cast<EnumType>(0)),  ...);
    return 0;
}

}  // namespace detail

}
