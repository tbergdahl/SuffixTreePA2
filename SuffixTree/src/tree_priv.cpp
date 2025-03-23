#include "tree.h"

// private methods for tree

namespace SuffixTree
{

void Tree::insert(std::string const& suffix)
{
    insert(root, suffix);
}


void Tree::insert(PNode const& node, std::string const& suffix)
{
    if(!root)
    {
        root = std::make_unique<Node>(node_count++);
    }
    else
    {

    }
}


}