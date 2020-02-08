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
    NODE_7 = 7,
    NODE_8 = 8,
    NODE_9 = 9,
    NODE_10 = 10,
    NODE_11 = 11
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

    // NodeTypes::NODE_2, NodeTypes::NODE_3,
    static constexpr auto graph = Graph{Node{NodeTypes::NODE_1, NodeTypes::NODE_8, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                        Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                        Node{NodeTypes::NODE_3, NodeTypes::NODE_4, NodeTypes::NODE_5},
                                        Node{NodeTypes::NODE_4},
                                        Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7},
                                        Node{NodeTypes::NODE_6},
                                        Node{NodeTypes::NODE_7},
                                        Node{NodeTypes::NODE_8, NodeTypes::NODE_9, NodeTypes::NODE_10},
                                        Node{NodeTypes::NODE_9},
                                        Node{NodeTypes::NODE_10, NodeTypes::NODE_11},
                                        Node{NodeTypes::NODE_11}};

    constexpr auto i = graph.path_exists(NodeTypes::NODE_1, NodeTypes::NODE_7);
    static_assert(i == 1, "");
    std::cout << i << "\n";
    
    return 0;
}
