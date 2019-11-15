#pragma once

#include "./ctgraph_std.hpp"
// #include "./ctgraph_traits.hpp"

namespace ctgraph
{
namespace detail
{

template<typename EnumType, size_t N>
class node_iterator
{
    // static_assert(std::is_enum_v<EnumType>, "The first argument of each node must be of enum type");
			
  public:
    using value_type = EnumType;
    using pointer    = EnumType*;
    using reference  = EnumType&;
    using difference_type = std::ptrdiff_t;

    constexpr node_iterator(): m_nodes(), m_index(0) {}   
    constexpr node_iterator(std::array<EnumType, N> nodes, int index): m_nodes(nodes), m_index(index) {}

    constexpr reference       operator*()             {return m_nodes[m_index];}
    constexpr pointer         operator->()            {return &m_nodes[m_index];}
	

    constexpr node_iterator& operator++()       {++m_index;return *this;}
    constexpr node_iterator  operator++(int)    {node_iterator r(*this);++m_index;return r;}

    constexpr bool operator!=(const node_iterator &r) const {return m_index != r.m_index;}
    constexpr bool operator==(const node_iterator &r) const {return m_index == r.m_index;}

  private:
    std::array<EnumType, N> m_nodes;
    int m_index;
};

		
} //namespace detail


template<typename Graph>
constexpr auto graph_begin(Graph &g)
{
	// static_assert(is_graph_v<Graph> , "begin requires a Graph as it first argument");
    return detail::node_iterator(g.vertices(), 0);
}


template<typename Graph>
constexpr auto graph_end(Graph &g)
{
	// static_assert(is_graph_v<Graph> , "begin requires a Graph as it first argument");
    return detail::node_iterator(g.vertices(), static_cast<int>(std::size(g.vertices())));
}
	

} //namespace ctgraph
