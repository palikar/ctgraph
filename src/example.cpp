#include <ctgraph.hpp>

enum class NodeTypes
{
    UNKNOWN = 0,
    NODE_1 = 1,
    NODE_2 = 2,
    NODE_3 = 3,
    NODE_4 = 4,
    NODE_5 = 5,
    NODE_6 = 6,
    NODE_7 = 7
};

// convenience functions for printing the value of the Nodes enum
template<typename EnumType, typename To=int>
To pr(EnumType node)
{
    return static_cast<To>(node);
}



std::ostream& operator<<(std::ostream& os, const NodeTypes& c)
{
    os << pr(c);
    return os;
}


int main(int, char**)
{
    using namespace ctgraph;

    // constructing a graph
    static constexpr auto graph = Graph{Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                        Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                        Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                        Node{NodeTypes::NODE_4},
                                        Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};
    // only the nodes NODE_1, NODE_2, NODE_3, NODE_4 and NODE_5 are considered 'in the graph'
    // if we also want to consider NODE_6 and NODE_7, we have to make entries for them, similar to NODE_4

    // getting the ith node in the graph
    constexpr auto node_0 = graph.get_node<0>();
    // count of the successor of a given node
    std::cout << node_0.m_cnt << "\n";

    // getting the vertiecies of the graph
    // vertices is std::array<NodeTypes, 5>
    constexpr auto vertices = graph.vertices(); 
    for(const auto& v: vertices ){
        std::cout << "Vertex: " << v << "\n";
    }

    // the count of all veriecies in the graph
    constexpr auto size = graph.size();
    std::cout << "Size: " << size << "\n";

    std::cout << std::boolalpha;

    // checking if a node is in the graph
    std::cout << graph.contains(NodeTypes::NODE_4) << "\n";
    std::cout << graph.contains(NodeTypes::NODE_6) << "\n";

    // checking if two nodes are adjacent in the graph
    // this means that the edge (node_1 -> node_2) exists
    std::cout << graph.adjacent(NodeTypes::NODE_2, NodeTypes::NODE_3) << "\n";
    std::cout << graph.adjacent(NodeTypes::NODE_2, NodeTypes::NODE_5) << "\n";
    
    // iterating over the successors of a particular node 
    graph.for_each(NodeTypes::NODE_2, [](const auto& node){ std::cout << "Node_2 successor: " << node << "\n";});

    // getting the ith successor of a node
    constexpr auto node_1_succesor_0 = graph.successor<0>(NodeTypes::NODE_1);
    std::cout << static_cast<int>(node_1_succesor_0) << "\n";

    // getting all successor of a node
    const auto[count, ptr] = get_successors(graph, NodeTypes::NODE_3);
    for (size_t i = 0; i < count; ++i)
    {
        std::cout << ptr[i] << "\n";
    }

    // getting the count of the successrs of a node
    constexpr auto num_succ = graph.count(NodeTypes::NODE_3);
    std::cout << num_succ << "\n";

    // getting pointer to the first succesor of a node
    static constexpr auto ptr_succ = graph.followers(NodeTypes::NODE_1);
    std::cout << static_cast<int>(ptr_succ[0]) << "\n";

    return 0;
}
