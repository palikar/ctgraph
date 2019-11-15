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

    
    static constexpr auto graph = Graph{Node{NodeTypes::NODE_1, NodeTypes::NODE_2},
                                        Node{NodeTypes::NODE_2}};

    auto beg = graph.begin();
    auto end = graph.end();

    std::cout << *beg << "\n";
    std::cout << *end << "\n";

    
    // for (auto n :  )
    // {
    //     std::cout << n << "\n";
    // }

    

    return 0;
}
