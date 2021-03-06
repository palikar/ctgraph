#pragma once

#include "./ctgraph_std.hpp"
#include "./ctgraph_traits.hpp"

namespace ctgraph
{

namespace detail
{

template<typename EnumType, size_t N>
class node_iterator
{
    static_assert(std::is_enum_v<EnumType>, "Iterators need enum type");
			
  public:
    using value_type = EnumType;
    using pointer    = EnumType*;
    using reference  = EnumType&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    constexpr node_iterator(): m_nodes(), m_index(0) {}   
    constexpr node_iterator(std::array<EnumType, N> nodes, size_t index): m_nodes(nodes), m_index(index) {}

    constexpr reference       operator*()             {return m_nodes[m_index];}
    constexpr pointer         operator->()            {return &m_nodes[m_index];}
	

    constexpr node_iterator& operator++()       {++m_index;return *this;}
    constexpr node_iterator  operator++(int)    {node_iterator r(*this);++m_index;return r;}

    constexpr bool operator!=(const node_iterator &r) const {return m_index != r.m_index;}
    constexpr bool operator==(const node_iterator &r) const {return m_index == r.m_index;}

  private:
    std::array<EnumType, N> m_nodes;
    size_t m_index;
};

template<typename EnumType, size_t N>
class cnode_iterator
{
    static_assert(std::is_enum_v<EnumType>, "Iterators need enum type");
			
  public:
    using value_type = EnumType;
    using pointer    = const EnumType*;
    using reference  = const EnumType&;
    using difference_type = size_t;
    using iterator_category = std::input_iterator_tag;

    constexpr cnode_iterator(): m_nodes(), m_index(0) {}   
    constexpr cnode_iterator(std::array<EnumType, N> nodes, size_t index): m_nodes(nodes), m_index(index) {}

    constexpr reference       operator*()             {return m_nodes[m_index];}
    constexpr pointer         operator->()            {return &m_nodes[m_index];}
	

    constexpr cnode_iterator& operator++()       {++m_index;return *this;}
    constexpr cnode_iterator  operator++(int)    {cnode_iterator r(*this);++m_index;return r;}

    constexpr bool operator!=(const cnode_iterator &r) const {return m_index != r.m_index;}
    constexpr bool operator==(const cnode_iterator &r) const {return m_index == r.m_index;}

  private:
    std::array<EnumType, N> m_nodes;
    size_t m_index;
};

		
} //namespace detail


template<typename Graph>
constexpr auto graph_begin(Graph &g)
{
	static_assert(is_graph_v<Graph> , "begin requires a Graph as it first argument");
    return detail::node_iterator(g.vertices(), 0);
}


template<typename Graph>
constexpr auto graph_end(Graph &g)
{
	static_assert(is_graph_v<Graph> , "begin requires a Graph as it first argument");
    return detail::node_iterator(g.vertices(), std::size(g.vertices()) - 1);
}


template<typename Graph>
constexpr auto graph_cbegin(Graph &g)
{
	static_assert(is_graph_v<Graph> , "begin requires a Graph as it first argument");
    return detail::cnode_iterator(g.vertices(), 0);
}


template<typename Graph>
constexpr auto graph_cend(Graph &g)
{
	static_assert(is_graph_v<Graph> , "begin requires a Graph as it first argument");
    return detail::cnode_iterator(g.vertices(), std::size(g.vertices()) - 1);
}
	

} //namespace ctgraph
