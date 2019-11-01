#include <ctgraph.hpp>
#include <catch2/catch.hpp>

enum class NodeTypes
{
    NODE_UNKNOWN = 0,
    NODE_1 = 1,
    NODE_2 = 2,
    NODE_3 = 3,
    NODE_4 = 4,
    NODE_5 = 5,
    NODE_6 = 6,
    NODE_7 = 7
};


namespace cfg = ctgraph;


TEST_CASE( "Get_followers", "[functions]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    auto[cnt_1, ptr_1] = get_successors(graph, NodeTypes::NODE_1);
    REQUIRE( cnt_1 == 2 );
    REQUIRE( ptr_1[0] == NodeTypes::NODE_2 );
    REQUIRE( ptr_1[1] == NodeTypes::NODE_3 );

    
    auto[cnt_3, ptr_3] = get_successors(graph, NodeTypes::NODE_3);
    REQUIRE( cnt_3 == 1 );
    REQUIRE( ptr_3[0] == NodeTypes::NODE_4 );
}
