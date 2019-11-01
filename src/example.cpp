
#include <ctgraph.hpp>

enum class NodeTypes
{
    NODE_1 = 1,
    NODE_2 = 2,
    NODE_3 = 3,
    NODE_4 = 4,
    NODE_5 = 5,
    NODE_6 = 6,
    NODE_7 = 7
};


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


    const auto graph = Graph{Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                             Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                             Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                             Node{NodeTypes::NODE_4},
                             Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}
    };


    auto vert = graph.vertices();
    for(const auto& v: vert ){
        std::cout << v << "\n";
    }
            
    std::cout << graph.size() << "\n";
    std::cout << std::boolalpha;

    std::cout << graph.contains(NodeTypes::NODE_4) << "\n";
    std::cout << graph.contains(NodeTypes::NODE_6) << "\n";

    std::cout << graph.adjacent(NodeTypes::NODE_2, NodeTypes::NODE_3) << "\n";
    std::cout << graph.adjacent(NodeTypes::NODE_2, NodeTypes::NODE_5) << "\n";
    
    std::cout << graph.size() << "\n";

    graph.for_each(NodeTypes::NODE_2, [](auto& node){
                                          std::cout << static_cast<int>(node) << "\n";
                                      });

    auto n = graph.successor<2>(NodeTypes::NODE_1);
    std::cout << static_cast<int>(n) << "\n";

    auto[cnt, ptr] = get_successors(graph, NodeTypes::NODE_3);
    for (size_t i = 0; i < cnt; ++i)
    {
        std::cout << static_cast<int>(ptr[i]) << "\n";
    }

    const auto num = graph.count(NodeTypes::NODE_3);
    std::cout << num << "\n";
    const auto ptr_2 = graph.followers(NodeTypes::NODE_1);
    std::cout << static_cast<int>(ptr_2[0]) << "\n";



    std::cout << graph.m_nodes_cnt << "\n";;
    auto nn = graph.get_node<0>();
    std::cout << nn.m_cnt << "\n";;

    


    return 0;
}
