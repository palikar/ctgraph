#include <ctgraph.hpp>
#include "./catch.hpp"

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





TEST_CASE( "Path predicate", "[algorithms]" ) {
    using namespace ctgraph;
    const auto graph = Graph{Node{NodeTypes::NODE_1, NodeTypes::NODE_8, NodeTypes::NODE_2, NodeTypes::NODE_3},
                             Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                             Node{NodeTypes::NODE_3, NodeTypes::NODE_4, NodeTypes::NODE_5},
                             Node{NodeTypes::NODE_4, NodeTypes::NODE_1},
                             Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7},
                             Node{NodeTypes::NODE_6},
                             Node{NodeTypes::NODE_7},
                             Node{NodeTypes::NODE_8, NodeTypes::NODE_9, NodeTypes::NODE_10},
                             Node{NodeTypes::NODE_9},
                             Node{NodeTypes::NODE_10, NodeTypes::NODE_11},
                             Node{NodeTypes::NODE_11, NodeTypes::NODE_7}};

    REQUIRE( graph.path_exists(NodeTypes::NODE_1, NodeTypes::NODE_4) );
    REQUIRE( graph.path_exists(NodeTypes::NODE_1, NodeTypes::NODE_7) );
    REQUIRE( !graph.path_exists(NodeTypes::NODE_5, NodeTypes::NODE_11) );

}
