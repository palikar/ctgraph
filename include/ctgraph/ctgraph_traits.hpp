#pragma once

#include "./ctgraph_std.hpp"

namespace ctgraph
{

template<typename EnumType1, typename... EnumType>
struct Node;

template<typename... NodeType>
struct Graph;

template<typename>
struct is_node : std::false_type {};

template<typename EnumType1, typename... EnumType>
struct is_node<Node<EnumType1, EnumType...>> : std::true_type {};

template<typename EnumType1, typename... EnumType>
struct is_node<const Node<EnumType1, EnumType...>> : std::true_type {};

template<typename>
struct is_graph : std::false_type {};

template<typename... NodeType>
struct is_graph<Graph<NodeType...>> : std::true_type {};

template<typename... NodeType>
struct is_graph<const Graph<NodeType...>> : std::true_type {};

template<typename T>
inline constexpr bool is_graph_v = is_graph<T>::value;

template<typename T>
inline constexpr bool is_node_v = is_node<T>::value;



// template<typename T>
// struct has_size_method_2
// {
//   private:
// 	typedef std::true_type yes;
// 	typedef std::false_type no;
 
// 	template<typename U> static auto test(int) -> decltype(std::declval<U>().size() == 1, yes());
//     template<typename> static no test(...);
 
//   public:
// 	static constexpr bool value = std::is_same_v<decltype(test<T>(0)), yes>;
// };


}

