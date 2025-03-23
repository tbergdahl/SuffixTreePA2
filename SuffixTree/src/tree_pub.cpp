#include "tree.h"

// Public methods for tree

namespace SuffixTree
{
constexpr char ESCAPE_CHAR = '$';

Tree Tree::build(std::string const& str, std::string const& alphabet)
{
    auto tree = Tree();
    tree.root = std::make_unique<Node>(0);

    // if any characters in the input are not a part of the alphabet
    if(str.find_first_not_of(alphabet) != std::string::npos)
    {
        return tree;
    }

    std::string input = str + ESCAPE_CHAR;

    for (auto i = 0; i < input.length(); i++)
    {
        auto suffix = input.substr(i);
        Node* curNode = &*tree.root;
        for (char c : suffix)
        {
            if (curNode->children.count(c) > 0)
            {
                curNode = &*(curNode->children[c]);
            }
            else
            {
                auto new_node = std::make_unique<Node>(i);
                curNode->children.emplace(c, std::move(new_node));
                curNode = &*(curNode->children[c]);
            }
        }
        curNode->type = NodeType::LEAF;
    }

    return tree;
}

void Tree::display_children() const
{
    display_children(root, "");
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