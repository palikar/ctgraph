#pragma once

#include "./ctgraph/ctgraph_std.hpp"
#include "./ctgraph/ctgraph_iterators.hpp"

namespace ctgraph
{
template<typename Graph, typename EnumType>
constexpr auto get_successors(Graph &g, EnumType val);
    
namespace detail
{

template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value)
{
    for (; first != last; ++first) {
        if (*first == value) {
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


}  // namespace detail


template<typename EnumType1, typename... EnumType>
struct Node
{
    static_assert(std::is_enum_v<EnumType1>, "The first argument of each node must be of enum type");
    static_assert(std::conjunction_v<std::is_enum<EnumType>...>, "The successors of each node must be of enum type");

    using value_type = EnumType1;

    const EnumType1 m_name;
    const std::array<EnumType1, sizeof...(EnumType)> m_tos;
    static constexpr size_t m_cnt = sizeof...(EnumType);

    constexpr explicit Node(const EnumType1 name, const EnumType... tos) : m_name(name), m_tos({ tos... }){};
};

template<size_t N, typename... Ts> using NthTypeOf =
                 typename std::tuple_element<N, std::tuple<Ts...>>::type;

template<typename... NodeType>
struct Graph
{
    static_assert(std::conjunction_v<is_node<NodeType>...>, "Non-Node argument in the Graph constructor!");

  private:
    const std::tuple<NodeType...> m_nodes;

  public:
    using size_type                        = decltype(std::tuple_size<decltype(m_nodes)>::value);
    static constexpr size_type m_nodes_cnt = sizeof...(NodeType);

    using iterator = detail::node_iterator<typename NthTypeOf<0, NodeType...>::value_type, sizeof...(NodeType)>;
    using const_iterator = detail::cnode_iterator<typename NthTypeOf<0, NodeType...>::value_type, sizeof...(NodeType)>;


    explicit constexpr Graph(const NodeType... nodes) : m_nodes(std::move(nodes)...)
    {
    }

    template<size_t Index>
    constexpr auto get_node() const
    {
        return std::get<Index>(m_nodes);
    }

    constexpr size_type size() const
    {
        return m_nodes_cnt;
    }

    template<typename EnumType>
    constexpr bool contains(EnumType val) const
    {
        return detail::contains_<0, sizeof...(NodeType)>(val, m_nodes);
    }

    constexpr auto vertices() const
    {
        return detail::vert_(m_nodes, std::make_index_sequence<m_nodes_cnt>());
    }

    template<typename EnumType>
    constexpr bool adjacent(EnumType from, EnumType to) const
    {
        return detail::adjacent_<0, sizeof...(NodeType)>(from, to, m_nodes);
    }

    template<size_t N, typename EnumType>
    constexpr EnumType successor(const EnumType val) const
    {
        return detail::get_node_impl<N, 0, sizeof...(NodeType)>(val, m_nodes);
    }

    template<typename EnumType>
    constexpr size_t count(const EnumType val) const
    {
        return detail::count_<0, sizeof...(NodeType)>(val, m_nodes);
    }

    template<typename EnumType, typename Callable>
    constexpr void for_each(const EnumType val, Callable &&call) const
    {
        return detail::for_each_<0, sizeof...(NodeType)>(val, m_nodes, std::forward<Callable>(call));
    }

    template<typename EnumType>
    constexpr const EnumType *followers(const EnumType val) const
    {

        constexpr auto application = [](auto &&arr) { return arr.data(); };
        return detail::apply_<0, sizeof...(NodeType)>(val, m_nodes, application);
    }

    constexpr iterator begin() const
    {
        return graph_begin(*this);
    }
    
    constexpr iterator end() const
    {
        return graph_end(*this);
    }

    constexpr const_iterator cbegin() const
    {
        return graph_cbegin(*this);
    }
    constexpr const_iterator cend() const
    {
        return graph_cend(*this);
    }



    
    // template<typename NthTypeOf<0, NodeType...>::value_type from,
    //          typename NthTypeOf<0, NodeType...>::value_type to,
    //          typename EnumType,
    //          size_t... Is, size_t vis = 0 , size_t... Js >
    // constexpr bool path_predicate(std::index_sequence<Is...>,
    //                               std::array<EnumType, vis> visited= {},
    //                               std::index_sequence<Js...> = {}) const
    // {
        

    //     constexpr std::array arr{ (std::tuple(std::get<Is>(m_nodes).m_name, std::get<Is>(m_nodes).m_cnt, std::get<Is>(m_nodes).m_tos.data()))...}; 
    //     for (const auto [node, cnt, tos] : arr) {
            
    //         if (node != from) {
    //             continue;
    //         }

    //         if (cnt==0) {
    //             return false;
    //         }

    //         if (detail::adjacent_<0, sizeof...(NodeType)>(from, to, m_nodes)) {
    //             return true;
    //             }
    //         else
    //         {
    //             constexpr std::array<EnumType, visited.size()+1> expanded{visited[Js]..., node};
                
    //             constexpr auto visited_sequence = std::make_index_sequence<visited.size()+1>();
        
    //             constexpr auto fun =
    //                 [=](auto ... I) {
    //                     return (path_predicate<*(tos + I), to, EnumType>(std::make_index_sequence<sizeof...(Is)>(), expanded, visited_sequence) || ...);
    //                 };
                    
    //             return fun(std::make_index_sequence<cnt>());
                    
                    
    //             }

    //     }
    //     return false;
    // }

    // template<typename NthTypeOf<0, NodeType...>::value_type from,
    //          typename NthTypeOf<0, NodeType...>::value_type to>
    // constexpr auto path_exists() const
    // {    
        
    //     return path_predicate<from, to, typename NthTypeOf<0, NodeType...>::value_type>(std::make_index_sequence<m_nodes_cnt>());

    // }
	
};


template<typename NthTypeOf<0, NodeType...>::value_type from,
         typename NthTypeOf<0, NodeType...>::value_type to,
         typename NodeType
         >
constexpr auto path_exists(Graph<NodeType ...> g)
{    


    return false;
    // return path_predicate<from, to, typename NthTypeOf<0, NodeType...>::value_type>(std::make_index_sequence<m_nodes_cnt>());

}


template<typename Graph, typename EnumType>
constexpr auto get_successors(Graph &g, EnumType val)
{
	static_assert(is_graph_v<Graph> , "get_successors requires a Graph as it first argument");
	return std::make_pair(g.count(val), g.followers(val));
}

}  // namespace ctgraph


// template<typename EnumType>
// NodeIterator() -> NodeIteratory<EnumType, void>
