#include "tree.h"
#include <iostream>

namespace SuffixTree
{

void Tree::find_path(Node* u, const std::string& s, int index) 
{
    Node* v = u;
    auto x = s.substr(index);

    if (x.empty()) return; // If the suffix is empty, we don't need to insert anything.

    while (true)
    {
        char firstChar = x[0];

        auto it = v->children.find(firstChar);
        if (it == v->children.end()) 
        {
            // No child starts with the first character, so we add a new leaf node.
            Node* leaf = new Node(index);
            leaf->start = index;
            leaf->end = -1; // Leaf node
            leaf->suffixIndex = index;
            v->children[firstChar] = leaf;
            break;
        }

        Node* existing = it->second;
        int suf_start = existing->start;
        int suf_end = (existing->end == -1 ? (s.length() - 1) : existing->end); // If it's a leaf, j goes to the end of s

        int j = suf_start;
        int i = index;

        // Compare the suffixes character by character
        for (; i < s.length() && j <= suf_end && s[i] == s[j]; i++, j++);

        bool is_mismatch = (i < s.length() && j <= suf_end && s[i] != s[j]);

        if (is_mismatch) 
        {
            // Mismatch: split the edge at the point of mismatch
            Node* internalNode = new Node(index);
            internalNode->start = suf_start;
            internalNode->end = j - 1; // Corrected: end is j - 1

            Node* leaf = new Node(index);
            leaf->start = i;
            leaf->end = -1; // Leaf node
            leaf->suffixIndex = index;

            existing->start = j; // Update the existing node's start position
            internalNode->children[s[j]] = existing;
            internalNode->children[s[i]] = leaf;

            v->children[firstChar] = internalNode; // Replace the existing node with the internal node

            break;
        } 
        else 
        {
            // Fully matched, continue checking with the next node
            v = existing;
        }
    }
}

void Tree::display(Node* node, const std::string& orig_str, int indent)
{
    // Print indentation
    for (int i = 0; i < indent; ++i)
    {
        std::cout << "  "; // Two spaces per level of indentation
    }

    if (node == root)
    {
        std::cout << "Root" << std::endl;
        for (auto child : node->children)
        {
            display(child.second, orig_str, indent + 1);
        }
    }
    else if (node->end == -1)
    {
        // Leaf node: print the suffix
        std::cout << "Leaf: " << orig_str.substr(node->start) << " (Label: " << node->suffixIndex << ")" << std::endl;
    }
    else
    {
        // Internal node: print the substring
        std::cout << "Internal: " << orig_str.substr(node->start, node->end - node->start + 1) << std::endl;

        // Recursively display children with increased indentation
        for (auto child : node->children)
        {
            display(child.second, orig_str, indent + 1);
        }
    }
}

} // namespace SuffixTree