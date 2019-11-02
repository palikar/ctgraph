Build status: [![Build Status](https://travis-ci.org/palikar/ctgraph.svg?branch=master)](https://travis-ci.org/palikar/ctgraph)
    [![codecov](https://codecov.io/gh/palikar/ctgraph/branch/master/graph/badge.svg)](https://codecov.io/gh/palikar/ctgraph)
    [![CodacyBadge](https://api.codacy.com/project/badge/Grade/83223760ee624b159b1869492c6d99ed)](https://www.codacy.com/manual/palikar/ctgraph?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=palikar/ctgraph&amp;utm_campaign=Badge_Grade)
    [![CodeFactor](https://www.codefactor.io/repository/github/palikar/ctgraph/badge)](https://www.codefactor.io/repository/github/palikar/ctgraph)
    [![ScoreMe](http://readme-score-api.herokuapp.com/score.svg?url=https%3a%2f%2fgithub.com%2fpalikar%2fctgraph%2f)](http://clayallsopp.github.io/readme-score?url=https%3a%2f%2fgithub.com%2fpalikar%2fctgraph%2f)


# Compile-Time Graph (CTGraph)

![img](./logo.png)


## Abstract

A small C++17 project that implements as graph structure that can be used entirely at compile time. I was mainly inspired by a part of [`cpp_box`](https://github.com/lefticus/cpp_box) &#x2013; a project by [Jason Turner](https://github.com/lefticus/cpp_box). In there he has implemented a finate-state machine that can be used at compile-time in a constexpr context. This had me fascinated and made me do some experimental programming in hopes of finding out what else can be achieved at compile time. Just like that, for several hours I botched together CTGraph (Compile-Time Graph).


### Seller features

-   Header only
-   Usable in constexpr context (mostly)
-   Usable in const context (fully)


## Installation


### Prerequisites

The project can be compiled with clang7, gcc9 and gcc8.


### Building

CTGraph can easily be build from source:

```sh
git clone https://github.com/palikar/ctgraph/
cd ctgraph && mkdir build
cd build
cmake .. -DALL_WARNINGS=1 -DLIBCPP=0 -DENABLE_CLANG_TIDY=0 -DENABLE_CPPCHECK=0 -DENABLE_COVERAGE=1 -DBUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Debug
make -j4
```

The flags for the cmake command can be adjusted as needed.



The build generates the file `ctgraph.hpp` in the build directory. This is a single header that can be included in other projects and provides all of the functionality of CTGraph.


### Running tests

CTGraph uses [Catch2](https://github.com/catchorg/Catch2) for its testing. If the cmake command was ran with `-DBUILD_TESTS=1`, there will be an executable `graph_tests` in the build directory. Running it will result in the execution of all tests for the project. Alternatively one can also use ctest:

```sh
ctest -j2
```

or invoke the `test` target

```sh
make test
```


## Usage

As mentioned, including the single header file in your file is enough to get you started. To note is that CTGraph uses some C++17 features so a compile flag like `-std=c++17` is required to compile a file using CTGraph.



The nodes for a graph are given through a user-defined `enum` like:

```c++
enum class NodeTypes
{
    UNKNOWN = 0,
    NODE_1 = 1,
    NODE_2 = 2,
    NODE_3 = 3
};
```

With that, we can now construct our graph in a &ldquo;static and constexpr&rdquo; way:

```c++
#include <ctgraph.hpp>
static constexpr auto graph = ctgraph::Graph{ctgraph::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                             ctgraph::Node{NodeTypes::NODE_2, NodeTypes::NODE_3}};
```

The semantics of the constructor of ctgraph::Node are `<starting_vertex> [<follower(s)>]`. Only nodes starting node are considered vertices in the graph. IN the above example, `NODE_3` is not &ldquo;in&rdquo; the vertex list of the graph. For this to be the case, we have to define the graph as:

```c++
static constexpr auto graph = ctgraph::Graph{ctgraph::Node{NodeTypes::NODE_1, NodeTypes::NODE_2, NodeTypes::NODE_3},
                                             ctgraph::Node{NodeTypes::NODE_2, NodeTypes::NODE_3},
                                             ctgraph::Node{NodeTypes::NODE_3}};
```

From here on out, we can use the graph in all sorts of ways.



Getting all of the vertices:

```c++
constexpr auto vertices = graph.vertices(); 
for(const auto& v: vertices ){
    std::cout << "Vertex: " << static_cast<int>(v) << "\n";
}
```



Checking the size of the graph:

```c++
constexpr auto size = graph.size();
std::cout << "Size: " << size << "\n";
```

The number of vertices is considered as &ldquo;size&rdquo;.



Checking if Node is in the graph:

```c++
std::cout << graph.contains(NodeTypes::NODE_4) << "\n";
```



Checking if two nodes are adjacent in the graph:

```c++
std::cout << graph.adjacent(NodeTypes::NODE_2, NodeTypes::NODE_3) << "\n";
std::cout << graph.adjacent(NodeTypes::NODE_3, NodeTypes::NODE_1) << "\n";
```



You can find a compilable and commented example in the [source folder](https://github.com/palikar/ctgraph/blob/master/src/example.cpp). It demonstrates most of the current features of the library.


## Acknowledgments

-   [Jason Turner](https://github.com/lefticus/cpp_box) and [`state_machine.hpp`](https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/state_machine.hpp) for inspiration.
-   [Amalgamate](https://github.com/edlund/amalgamate) for generating a single-include header file.
