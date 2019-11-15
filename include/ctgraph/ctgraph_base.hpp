#pragma once


#include "./ctgraph_std.hpp"
#include "./ctgraph_traits.hpp"
#include "./ctgraph_iterators.hpp"

namespace ctgraph
{

namespace detail
{

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
            return std::find(std::begin(node.m_tos), std::end(node.m_tos), to) != std::end(node.m_tos);
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


template<typename... NodeType>
struct Graph
{
    static_assert(std::conjunction_v<is_node<NodeType>...>, "Non-Node argument in the Graph constructor!");

  private:
    const std::tuple<NodeType...> m_nodes;

  public:
    using size_type                        = decltype(std::tuple_size<decltype(m_nodes)>::value);
    static constexpr size_type m_nodes_cnt = sizeof...(NodeType);


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
    constexpr bool adjacent(EnumType val, EnumType to) const
    {
        return detail::adjacent_<0, sizeof...(NodeType)>(val, to, m_nodes);
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


	  constexpr auto begin() const {this->vertices().begin()}
	  constexpr auto end() const {this->vertices().end()}
	  constexpr auto cbegin() const {this->vertices().cbegin()}
	  constexpr auto cend() const {this->vertices().cend()}
	
	
};


ptemplate<typename Graph, typename EnumType>
constexpr auto get_successors(Graph &g, EnumType val)
{
	static_assert(is_graph_v<Graph> , "get_successors requires a Graph as it first argument");
	return std::make_pair(g.count(val), g.followers(val));
}

}  // namespace ctgraph


// template<typename EnumType>
// NodeIterator() -> NodeIteratory<EnumType, void>
