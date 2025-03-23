#ifndef TREE
#define TREE

#include "node.h"


#include <iostream>
#include <string>

namespace SuffixTree
{
using PNode = std::unique_ptr<Node>;
class Tree
{
public:
    static Tree build(std::string const& str, std::string const& alphabet);

    void display_children(PNode const& node) const;

    void enumerate_nodes() const;

    void bwt_for(std::string const& str) const;
private:
    Tree() = default;

    void insert(std::string const& suffix);

    void insert(PNode const& node, std::string const& suffix);

    PNode root;

    unsigned node_count;

};

}

#endif