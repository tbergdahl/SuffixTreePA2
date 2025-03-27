#include "tree.h"
#include <iostream>

namespace SuffixTree
{
    Node* Tree::last_inserted_leaf = nullptr;

    void Tree::reset()
    {
        last_inserted_leaf = nullptr;
    }

    void Tree::find_path(Node* u, const std::string& s, int index) {
        Node* v = u;
        int matched = 0; // Track total matched characters from the suffix
        auto x = s.substr(index); // Remaining suffix to insert
    
        if (x.empty()) return;
    
        while (true) {
            char firstChar = x[0];
            auto it = v->children.find(firstChar);
    
            // Case 1: No existing edge - create new leaf
            if (it == v->children.end()) {
                Node* leaf = new Node();
                leaf->start = index + matched; // Correctly offset by total matched
                leaf->end = -1;
                leaf->suffixIndex = index;
                leaf->parent = v;
                leaf->string_depth = v->string_depth + x.length();
                v->children[firstChar] = leaf;
                last_inserted_leaf = leaf;
                leaf_node_count++;
                break;
            }
    
            Node* existing = it->second;
            int suf_start = existing->start;
            int suf_end = (existing->end == -1) ? (s.length() - 1) : existing->end;
            int j = suf_start;
            int i = 0;
    
            // Compare characters until mismatch or edge exhausted
            while (i < x.size() && j <= suf_end && x[i] == s[j]) {
                i++;
                j++;
            }
    
            // Case 2: Entire edge matched - proceed to child node
            if (j > suf_end) {
                v = existing;
                matched += i; // Accumulate matched characters
                x = x.substr(i); // Update remaining suffix
                continue; // Process next part of the suffix
            }
    
            // Case 3: Mismatch within edge - split edge
            if (x[i] != s[j]) {
                // Create internal node
                Node* internalNode = new Node();
                internalNode->start = suf_start;
                internalNode->end = suf_start + i - 1;
                internalNode->parent = v;
                internalNode->string_depth = v->string_depth + i;
    
                // Adjust existing node (now child of internal node)
                existing->start = internalNode->end + 1;
                existing->parent = internalNode;
    
                // Create new leaf for remaining suffix
                Node* leaf = new Node();
                leaf->start = index + matched + i; // Correct start with total matched
                leaf->end = -1;
                leaf->suffixIndex = index;
                leaf->parent = internalNode;
                leaf->string_depth = internalNode->string_depth + (x.size() - i);
    
                // Update parent-child relationships
                v->children[firstChar] = internalNode;
                internalNode->children[s[existing->start]] = existing;
                internalNode->children[x[i]] = leaf;
    
                // Update metadata
                last_inserted_leaf = leaf;
                leaf_node_count++;
                break;
            }
        }
    }

    void Tree::build_tree(const std::string& input, int pos)
    {
        if (!last_inserted_leaf) last_inserted_leaf = root;

        Node* u = last_inserted_leaf->parent;
        if (!u) u = root;  // Handle case where last_inserted_leaf is root

        if (u->suffix_link)
        {
            // Follow the suffix link
            u = u->suffix_link;
            find_path(u, input, pos + u->string_depth);
        }
        else
        {
            // No suffix link, hop to the right node
            Node* grandparent = u->parent;
            Node* v = grandparent == root ? root : grandparent->suffix_link;
            v = node_hop(v, u->string_depth - (grandparent == root ? 0 : grandparent->string_depth), input);

            // Insert using find_path
            find_path(v, input, pos + v->string_depth);
            u->suffix_link = v;  // Set suffix link for future use
        }
    }

    Node* Tree::node_hop(Node* n, int depth, const std::string& s)
    {
        if (!n) return root;

        while (depth > 0 && n != root)
        {
            int edge_len = n->end - n->start + 1;
            if (depth >= edge_len)
            {
                depth -= edge_len;
                char next_char = s[n->start + edge_len];  // Next character after this edge
                auto it = n->children.find(next_char);
                n = it->second;
            }
            else
            {
                break;  // Target is within the current edge
            }
        }
        return n;
    }

    void Tree::display(Node* node, const std::string& orig_str, int indent)
    {
        for (int i = 0; i < indent; ++i)
        {
            std::cout << "  ";
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
            std::cout << "Leaf: " << orig_str.substr(node->start) << " (Label: " << node->suffixIndex << ")" << std::endl;
        }
        else
        {
            std::cout << "Internal: " << orig_str.substr(node->start, node->end - node->start + 1) << std::endl;
            for (auto child : node->children)
            {
                display(child.second, orig_str, indent + 1);
            }
        }
    }
}
