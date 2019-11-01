#include <catch2/catch.hpp>

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

TEST_CASE( "Basic construction", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    REQUIRE(graph.m_nodes_cnt == graph.m_nodes_cnt);
}

TEST_CASE( "Vertices getter ", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};


    auto vert = graph.vertices();
    REQUIRE(vert[0] == NodeTypes::NODE_1);
    REQUIRE(vert[1] == NodeTypes::NODE_2);
    REQUIRE(vert[2] == NodeTypes::NODE_3);
    REQUIRE(vert[3] == NodeTypes::NODE_4);
    REQUIRE(vert[4] == NodeTypes::NODE_5);
    REQUIRE(std::size(vert) == 5);
}

TEST_CASE( "Size getter ", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    REQUIRE( graph.size() == 5);
}

TEST_CASE( "Contains predicate", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    REQUIRE( graph.contains(NodeTypes::NODE_1) );
    REQUIRE( graph.contains(NodeTypes::NODE_2) );
    REQUIRE( graph.contains(NodeTypes::NODE_3) );
    REQUIRE( graph.contains(NodeTypes::NODE_4) );
    REQUIRE( graph.contains(NodeTypes::NODE_5) );
    REQUIRE( not graph.contains(NodeTypes::NODE_UNKNOWN) );
}


TEST_CASE( "Adjacents predicate", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};


    REQUIRE( graph.adjacent(NodeTypes::NODE_1, NodeTypes::NODE_2) );
    REQUIRE( graph.adjacent(NodeTypes::NODE_1, NodeTypes::NODE_3) );
    REQUIRE( graph.adjacent(NodeTypes::NODE_2, NodeTypes::NODE_3) );
    REQUIRE( graph.adjacent(NodeTypes::NODE_2, NodeTypes::NODE_4) );
    REQUIRE( graph.adjacent(NodeTypes::NODE_3, NodeTypes::NODE_4) );

    REQUIRE( not graph.adjacent(NodeTypes::NODE_4, NodeTypes::NODE_5) );
    REQUIRE( not graph.adjacent(NodeTypes::NODE_4, NodeTypes::NODE_4) );
    REQUIRE( not graph.adjacent(NodeTypes::NODE_5, NodeTypes::NODE_1) );
}

TEST_CASE( "For_each", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    int node_2_followers = 0;
    graph.for_each(NodeTypes::NODE_2, [&node_2_followers](auto&){node_2_followers++;});
    REQUIRE( node_2_followers == 2);

    int node_3_followers = 0;
    graph.for_each(NodeTypes::NODE_3, [&node_3_followers](auto&){node_3_followers++;});
    REQUIRE( node_3_followers == 1);

    int index = 0;
    NodeTypes node_1_followers[2];
    graph.for_each(NodeTypes::NODE_1, [&node_1_followers, &index](const auto& node){ node_1_followers[index++] = node;});
    REQUIRE(node_1_followers[0] == NodeTypes::NODE_2);
    REQUIRE(node_1_followers[1] == NodeTypes::NODE_3);


}


TEST_CASE( "Successor getter", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    auto n_2 = graph.successor<0>(NodeTypes::NODE_1);
    auto n_3 = graph.successor<1>(NodeTypes::NODE_1);
    REQUIRE( n_2 == NodeTypes::NODE_2 );
    REQUIRE( n_3 == NodeTypes::NODE_3 );

    n_2 = graph.successor<0>(NodeTypes::NODE_2);
    n_3 = graph.successor<1>(NodeTypes::NODE_2);
    REQUIRE( n_2 == NodeTypes::NODE_3 );
    REQUIRE( n_3 == NodeTypes::NODE_4 );

}

TEST_CASE( "Count getter", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    REQUIRE( graph.count(NodeTypes::NODE_1) == 2 );
    REQUIRE( graph.count(NodeTypes::NODE_2) == 2 );
    REQUIRE( graph.count(NodeTypes::NODE_3) == 1 );
    REQUIRE( graph.count(NodeTypes::NODE_4) == 0 );
    REQUIRE( graph.count(NodeTypes::NODE_5) == 2 );
}

TEST_CASE( "Followers getter", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    REQUIRE( graph.followers(NodeTypes::NODE_1)[0] == NodeTypes::NODE_2 );
    REQUIRE( graph.followers(NodeTypes::NODE_1)[1] == NodeTypes::NODE_3 );
    REQUIRE( graph.followers(NodeTypes::NODE_2)[0] == NodeTypes::NODE_3 );
    REQUIRE( graph.followers(NodeTypes::NODE_2)[1] == NodeTypes::NODE_4 );
    REQUIRE( graph.followers(NodeTypes::NODE_3)[0] == NodeTypes::NODE_4 );
    REQUIRE( graph.followers(NodeTypes::NODE_5)[0] == NodeTypes::NODE_6 );
    REQUIRE( graph.followers(NodeTypes::NODE_5)[1] == NodeTypes::NODE_7 );

}

TEST_CASE( "Get_node getter", "[basic]" ) {

    const auto graph = cfg::Graph{cfg::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                  cfg::Node{NodeTypes::NODE_2, NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_3, NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_4},
                                  cfg::Node{NodeTypes::NODE_5, NodeTypes::NODE_6, NodeTypes::NODE_7}};

    REQUIRE( graph.get_node<0>().m_cnt == 2);
    REQUIRE( graph.get_node<1>().m_cnt == 2);
    REQUIRE( graph.get_node<2>().m_cnt == 1);
    REQUIRE( graph.get_node<3>().m_cnt == 0);
    REQUIRE( graph.get_node<4>().m_cnt == 2);

}

