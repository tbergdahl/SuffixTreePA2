#include "node.h"

#include <iostream>

namespace SuffixTree
{

Node::Node(unsigned n_id)
{
    id = n_id;
}

void Node::print()
{
    std::cout << id << std::endl;
}

}