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
			static_assert(std::is_enum_v<EnumType1>, "The first argument of each node must be of enum type");
			
		public:
			using value_type = EnumType;
			using pointer    = EnumType*;
			using reference  = EnumType&;
			using difference_type = std::ptrdiff_t;

			constexpr NodeIterator(): m_nodes(), m_index(0) {}   
			constexpr NodeIterator(EnumType* m_nodes, int index): m_nodes(nodes), m_index(index) {}

			constexpr reference       operator*()             {return m_nodes[m_index];}
			constexpr pointer         operator->()            {return &m_nodes[m_index];}
	

			constexpr NodeIterator& operator++()       {++m_index;return *this;}
			constexpr NodeIterator  operator++(int)    {NodeIterator r(*this);++m_index;return r;}

			constexpr bool operator!=(const NodeIterator &r) const {return m_index != r.m_index;}
			constexpr bool operator==(const NodeIterator &r) const {return m_index == r.m_index;}

		private:
			EnumType* m_nodes;
			int m_index;
		};

		
	} //namespace detail



	// template<typename Graph>
	// constexpr auto begin(Graph &g)
	// {
	// 	static_assert(is_graph_v<Graph> , "begin requires a Graph as it first argument");

	// 	return detail::node_iterator(g);
	// }
	

} //namespace ctgraph
