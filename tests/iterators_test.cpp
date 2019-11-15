#include "./catch.hpp"

#include <iterator>

#include <ctgraph.hpp>

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

TEST_CASE( "Basic iterators", "[iterators]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    
    REQUIRE( *graph.begin() == NodeTypes::NODE_1);
    REQUIRE( *(++graph.begin()) == NodeTypes::NODE_2);
    REQUIRE( *(++(++graph.begin())) == NodeTypes::NODE_3);
    REQUIRE( *(++(++(++graph.begin()))) == NodeTypes::NODE_4);    
}

TEST_CASE( "Std integration", "[iterators]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    

    REQUIRE( *std::begin(graph) == NodeTypes::NODE_1);
    REQUIRE( *std::end(graph) == NodeTypes::NODE_5);
    
    auto beg = graph.begin();
    auto end = graph.end();
    REQUIRE( *std::next(beg, 1) == NodeTypes::NODE_2);
    REQUIRE( *std::next(beg, 2) == NodeTypes::NODE_3);
    REQUIRE( *std::next(beg, 3) == NodeTypes::NODE_4);
    REQUIRE( *std::next(beg, 4) == NodeTypes::NODE_5);

    REQUIRE( std::distance(beg, end) == 4);    

}

