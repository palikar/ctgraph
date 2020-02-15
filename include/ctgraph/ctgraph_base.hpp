#pragma once

#include "./ctgraph/ctgraph_std.hpp"
#include "./ctgraph/ctgraph_iterators.hpp"
#include "./ctgraph/ctgraph_algorithms.hpp"
#include "./ctgraph/ctgraph_map.hpp"
#include "./ctgraph/ctgraph_vector.hpp"
#include "./ctgraph/ctgraph_detail.hpp"

namespace ctgraph
{
template<typename Graph, typename EnumType>
constexpr auto get_successors(Graph &g, EnumType val);


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

template<size_t N, typename... Ts>
using NthTypeOf = typename std::tuple_element<N, std::tuple<Ts...>>::type;

template<typename... NodeType>
struct Graph
{
    static_assert(std::conjunction_v<is_node<NodeType>...>, "Non-Node argument in the Graph constructor!");

  private:
    const std::tuple<NodeType...> m_nodes;

  public:
    using size_type                        = decltype(std::tuple_size<decltype(m_nodes)>::value);
    static constexpr size_type m_nodes_cnt = sizeof...(NodeType);

    using iterator       = detail::node_iterator<typename NthTypeOf<0, NodeType...>::value_type, sizeof...(NodeType)>;
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

    template<typename EnumType>
    constexpr auto path_exists(EnumType from, EnumType to) const
    {
        
        return detail::path_<EnumType, decltype(m_nodes), NodeType...>(
            from, to, m_nodes,
            detail::fill_visited_<EnumType, decltype(m_nodes), NodeType...>(m_nodes, std::make_index_sequence<m_nodes_cnt>()),
            std::make_index_sequence<m_nodes_cnt>());   
    }

    template<bool call_after = false, typename EnumType, typename Callable>
    constexpr void dfs( EnumType val, Callable  &&call) const
    {
        auto visited = detail::fill_visited_<EnumType, decltype(m_nodes), NodeType...>(m_nodes, std::make_index_sequence<m_nodes_cnt>());
        detail::dfs_<EnumType, decltype(m_nodes), call_after, NodeType...>(
            val, std::forward<Callable>(call), m_nodes,
            visited,
            std::make_index_sequence<m_nodes_cnt>());
    }

    template<typename EnumType, typename Callable>
    constexpr void bfs( EnumType val, Callable  call) const
    {
        auto visited = detail::fill_visited_<EnumType, decltype(m_nodes), NodeType...>(m_nodes, std::make_index_sequence<m_nodes_cnt>());
        cx::vector<EnumType, m_nodes_cnt> vec{};
        vec.push_back(val);
        visited[val] = true;
        while (!vec.empty()) {
            auto node = vec.pop_front();
            call(node);
            auto fol = followers(node);
            auto cnt = count(node);
            
            for (size_t i = 0; i < cnt; ++i) {
                if (visited[*(fol+i)]) { continue; }
                visited[*(fol+i)] = true;
                vec.push_back(*(fol+i));
            }
        }
    }

    template<typename EnumType>
    constexpr auto topological_sort(EnumType t_vertex) const
    {
        std::array<EnumType, m_nodes_cnt> top_sort{};
        size_t index = 0;

        auto sorter_dfs = [&top_sort, &index](auto node) {
            top_sort[index++] = node;
        };
        
        dfs<true>(t_vertex, sorter_dfs);

        return top_sort;
        
    }

    constexpr auto sources_count() const
    {
        return detail::sources_<typename NthTypeOf<0, NodeType...>::value_type>(m_nodes, std::make_index_sequence<m_nodes_cnt>());
    }

    constexpr auto sinks_count() const
    {
        return detail::sinks_(m_nodes, std::make_index_sequence<m_nodes_cnt>());
    }

    template<size_t... Is>
    constexpr auto _mother_vertex(std::index_sequence<Is...>) const
    {
        cx::map<typename NthTypeOf<0, NodeType...>::value_type, bool, m_nodes_cnt> visited{};
        typename NthTypeOf<0, NodeType...>::value_type last{};
        
        const std::array arr{ (std::get<Is>(m_nodes).m_name) ... };
        for (auto v : arr) { visited[v] = false; }

        const auto visitor = [&visited](auto node){
            visited[node] = true;
        };

        for (auto v : arr) {
            if (!visited[v]) {
                dfs<true>(v, visitor);
                last = v;
            }
        }

        for (auto v : arr) { visited[v] = false; }
        dfs<true>(last, visitor);

        for (auto v : arr) {
            if (!visited[v]) {
                return std::pair{last, false};
            }
        }
        
        return std::pair{last, true};
    }
    
    constexpr auto mother_vertex() const
    {
        return _mother_vertex(std::make_index_sequence<m_nodes_cnt>());
    }
    
    template<typename... NodeTypee>
    friend constexpr auto sinks(Graph<NodeTypee...> g);

    template<typename... NodeTypee>
    friend constexpr auto sources(Graph<NodeTypee...> g);
};

template<typename... NodeType>
constexpr auto sinks(Graph<NodeType...> g )
{
    auto const sinks_cout = g.sinks_count();
    std::array<typename NthTypeOf<0, NodeType...>::value_type, sinks_cout> arr{};
    detail::fill_sinks_(g.m_nodes, arr, std::make_index_sequence<g.m_nodes_cnt>());
    return arr;
}

template<typename... NodeType>
constexpr auto sources(Graph<NodeType...> g )
{
    const auto sink_nodes = detail::source_nodes<typename NthTypeOf<0, NodeType...>::value_type,
                                                 decltype(g.m_nodes),
                                               NodeType ...>(g.m_nodes, std::make_index_sequence<g.m_nodes_cnt>());

    const auto i = static_cast<size_t>(g.sources_count());
    
    return std::pair{sink_nodes, i};
}


template<typename Graph, typename EnumType>
constexpr auto get_successors(Graph &g, EnumType val)
{
    static_assert(is_graph_v<Graph>, "get_successors requires a Graph as it first argument");
    return std::make_pair(g.count(val), g.followers(val));
}

}  // namespace ctgraph
