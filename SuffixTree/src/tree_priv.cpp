#include "tree.h"
#include <iostream>

namespace SuffixTree
{

void Tree::find_path(Node* u, const std::string& s, int index) 
{
    Node* v = u;
    auto x = s.substr(index);

    if (x.empty()) return; // ff the suffix is empty, we don't need to insert anything

    while (true)
    {
        char firstChar = x[0];

        // search for child containing suffix that matches first char of the current suffix
        auto it = v->children.find(firstChar);
        if (it == v->children.end()) 
        {
            // child does not exist, so create
            Node* leaf = new Node(index);
            leaf->start = index;
            leaf->end = -1; // Leaf node, so suffix goes all the way to end of s
            leaf->suffixIndex = index;
            v->children[firstChar] = leaf; // add to tree
            break;
        }

        // if we reach this, there is a child who has a suffix starting with the same character as our current suffix.
        // now, we need to compare the suffixes to find mismatch, to potentially split the nodes.

        // get the child's slice of the string representing the suffix
        Node* existing = it->second;
        int suf_start = existing->start;
        int suf_end = (existing->end == -1 ? (s.length() - 1) : existing->end); // If it's a leaf, j goes to the end of s

        int j = suf_start;
        int i = index;

        // Compare the suffixes character by character
        for (; i < s.length() && j <= suf_end && s[i] == s[j]; i++, j++);

         
        /*
            all three need to be true for a mismatch:

            - the suffix we are inserting needs to have remaining characters
            
            - the child's suffix needs to have remaining characters, 
              or be right at the end (which implies our inserted suffix is an extension of the current suffix)

            - any character from the two didn't match at some point
        */
        bool is_mismatch = (i < s.length() && j <= suf_end && s[i] != s[j]);

        if (is_mismatch) // now, create an internal node which holds what was common between insert and existing suffixes
        {
            // internal node which holds the common suffix between the two
            Node* internalNode = new Node(index);
            internalNode->start = suf_start;
            internalNode->end = j - 1; // recall above: j will be one past the last matching character

            Node* leaf = new Node(index);
            leaf->start = i; // i is already one past the last matching character of the suffix to insert
            leaf->end = -1;
            leaf->suffixIndex = index;

            // Update the existing node's start position to be the character after the last matching character from existing suffix
            existing->start = j; 

            // add the nodes containing the non-matching pieces of their respective suffixes as children to the node 
            // containing their common suffix
            internalNode->children[s[j]] = existing;
            internalNode->children[s[i]] = leaf;

            v->children[firstChar] = internalNode; // insert into tree

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