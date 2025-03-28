#include "tree.h"

// Public methods for tree

namespace SuffixTree
{
constexpr char ESCAPE_CHAR = '$';

Tree Tree::build(std::string const& str, std::string const& alphabet)
{
    auto tree = Tree();
    // if any characters in the input are not a part of the alphabet, return empty tree
    if(str.find_first_not_of(alphabet) != std::string::npos)
    {
        return tree;
    }

    std::string input = str + ESCAPE_CHAR;
    for (int i = 0; i < input.length(); i++) {
        tree.insert_suffix(input, i);
    }

    return tree;
}

void Tree::enumerate_nodes() const
{
    // todo
}

void Tree::bwt_for(std::string const& str) const
{
    // todo
}

unsigned Tree::leaf_node_count() const
{
    return num_leaf_nodes;
}

// returns the number of internal nodes in the tree
unsigned Tree::internal_node_count() const
{
    return num_internal_nodes;
}

unsigned Tree::total_node_count() const
{
    return leaf_node_count() + internal_node_count();
}

unsigned Tree::average_string_depth() const
{
    return 1; // todo
}

unsigned Tree::deepest_string_depth() const
{
    return 1; // todo 
}

}