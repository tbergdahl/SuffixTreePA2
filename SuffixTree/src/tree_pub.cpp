#include "tree.h"

// Public methods for tree

namespace SuffixTree
{
constexpr char ESCAPE_CHAR = '$';

Tree Tree::build(std::string const& str, std::string const& alphabet)
{
    auto tree = Tree();

    std::string input = str + ESCAPE_CHAR;

    for (auto i = (input.length() - 1); i >= 0; i--)
    {
        tree.insert(input.substr(i));
    }

    return tree;
}

void Tree::display_children(PNode const& node) const
{
    // todo
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