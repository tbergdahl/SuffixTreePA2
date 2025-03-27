#include "tree.h"

// Public methods for tree

namespace SuffixTree
{
constexpr char ESCAPE_CHAR = '$';

Tree Tree::build(std::string const& str, std::string const& alphabet)
{
    auto tree = Tree();
    tree.root = new Node();
    tree.root->start = 0;
    tree.root->end = 0;
    tree.root->suffix_link = tree.root;
    tree.root->parent = tree.root;
    tree.reset();

    // if any characters in the input are not a part of the alphabet
    if(str.find_first_not_of(alphabet) != std::string::npos)
    {
        return tree;
    }

    std::string input = str + ESCAPE_CHAR;
    tree.user_input = input; // for debug
    for (int i = 0; i < input.length(); i++) {
        tree.build_tree(input, i);
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