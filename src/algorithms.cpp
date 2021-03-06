#include <ctgraph.hpp>

enum class NodeTypes
{
    UNKNOWN = 0,
    NODE_1  = 1,
    NODE_2  = 2,
    NODE_3  = 3,
    NODE_4  = 4,
    NODE_5  = 5,
    NODE_6  = 6,
    NODE_7  = 7,
    NODE_8  = 8,
    NODE_9  = 9,
    NODE_10 = 10,
    NODE_11 = 11,
    NODE_12 = 12
};

// convenience functions for printing the value of the Nodes enum
template<typename EnumType, typename To = int>
To pr(EnumType node)
{
    return static_cast<To>(node);
}


std::ostream &operator<<(std::ostream &os, const NodeTypes &c)
{
    os << pr(c);
    return os;
}


int main(int, char **)
{
    using namespace ctgraph;

    static constexpr auto graph = Graph{ Node{ NodeTypes::NODE_1, NodeTypes::NODE_8, NodeTypes::NODE_2, NodeTypes::NODE_3 },
                                         Node{ NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4 },
                                         Node{ NodeTypes::NODE_3, NodeTypes::NODE_4, NodeTypes::NODE_5 },
                                         Node{ NodeTypes::NODE_4, NodeTypes::NODE_6 },
                                         Node{ NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7 },
                                         Node{ NodeTypes::NODE_6 },
                                         Node{ NodeTypes::NODE_7 },
                                         Node{ NodeTypes::NODE_8, NodeTypes::NODE_9, NodeTypes::NODE_10 },
                                         Node{ NodeTypes::NODE_9 },
                                         Node{ NodeTypes::NODE_10, NodeTypes::NODE_11 },
                                         Node{ NodeTypes::NODE_11 } };

    constexpr auto i = graph.path_exists(NodeTypes::NODE_1, NodeTypes::NODE_7);
    static_assert(i == 1, "");


    constexpr auto cnt = graph.sinks_count();
    static_assert(cnt == 4, "");

    std::cout << "Sinks:"
              << "\n";
    constexpr auto s = sinks(graph);
    static_assert(s[0] == NodeTypes::NODE_6, "");
    static_assert(s[1] == NodeTypes::NODE_7, "");
    static_assert(s[2] == NodeTypes::NODE_9, "");
    static_assert(s[3] == NodeTypes::NODE_11, "");

    for (auto sor : s)
    {
        std::cout << "Sink: " << sor << "\n";
    }


    std::cout << "Sources:"
              << "\n";
    constexpr auto sinks_count = sources(graph);

    static_assert(sinks_count.first[0] == NodeTypes::NODE_1, "");

    for (size_t ind = 0; ind < sinks_count.second; ++ind)
    {
        std::cout << "Source: " << sinks_count.first[ind] << "\n";
    }

    std::cout << "DFS:"
              << "\n";
    graph.dfs(NodeTypes::NODE_1, [](auto node) { std::cout << "Node:" << node << "\n"; });

    std::cout << "BFS:"
              << "\n";
    graph.bfs(NodeTypes::NODE_1, [](auto node) { std::cout << "Node:" << node << "\n"; });

    constexpr auto top = graph.topological_sort(NodeTypes::NODE_1);
    static_assert(top[0] == NodeTypes::NODE_9, "");
    static_assert(top[1] == NodeTypes::NODE_11, "");
    static_assert(top[2] == NodeTypes::NODE_10, "");

    for (auto t : top)
    {
        std::cout << "Node: " << t << "\n";
    }

    constexpr auto mother = graph.mother_vertex();
    static_assert(mother.first == NodeTypes::NODE_1, "");
    std::cout << "Mother vertex" << mother.first << "\n";

    return 0;
}
