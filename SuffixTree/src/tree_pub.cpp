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

}