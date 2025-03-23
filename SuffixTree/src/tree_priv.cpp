#include "tree.h"

// private methods for tree

namespace SuffixTree
{

void Tree::display_children(const PNode& node, std::string current_str) const
{
    if (!node) return;

    // If it's a leaf node, print the path and index
    if (node->type == NodeType::LEAF)
    {
        std::cout << "Suffix: " << current_str << " (Index: " << node->id << ")\n";
    }

    // Recursively display children
    for (const auto& child : node->children)
    {
        char edge_char = child.first;
        display_children(child.second, current_str + edge_char);
    }
}



}