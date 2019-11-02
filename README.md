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


## Acknowledgments

-   [Jason Turner](https://github.com/lefticus/cpp_box) and [`state_machine.hpp`](https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/state_machine.hpp) for inspiration.
-   [Amalgamate](https://github.com/edlund/amalgamate) for generating a single-include header file.
