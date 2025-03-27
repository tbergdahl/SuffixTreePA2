#include "tree.h"
#include <iostream>

namespace SuffixTree
{
    Node* Tree::last_inserted_leaf = nullptr;

    void Tree::reset()
    {
        last_inserted_leaf = nullptr;
    }

void Tree::find_path(Node* u, const std::string& s, int index)
{
    Node* v = u;
    auto x = s.substr(index);  // Substring from the current index

    if (x.empty()) return;

    while (true)
    {
        // find child branch starting with x[1] (0 in code)
        char firstChar = x[0];
        auto it = v->children.find(firstChar);

        // If such branch exists
        if (it == v->children.end())
        {
            // insert leaf for s
            Node* leaf = new Node();
            leaf->start = index;
            leaf->end = -1;  // End is -1 to indicate it's a leaf node
            leaf->suffixIndex = index;
            leaf->parent = v;
            leaf->string_depth = index;
            v->children[firstChar] = leaf;
            last_inserted_leaf = leaf;
            leaf_node_count++;
            break;
        }

        
        Node* existing = it->second;
        int suf_start = existing->start;
        int suf_end = (existing->end == -1 ? (s.length() - 1) : existing->end);
        int j = suf_start;
        int i = 0;

        // compare chars of edge label against x[1..] until first mismatch or edge label exhuasted
        while (x[i] == s[j] && j <= suf_end) {
            i++;
            j++;
        }

        if(x[i] != s[j])
        {
            Node* internalNode = new Node();
            internalNode->start = existing->start;
            internalNode->end = existing->start + i - 1;  // Split at length i from edge start
            internalNode->parent = v;
            internalNode->string_depth = v->string_depth + i;
            std::cout << "New internal node created: (" << std::to_string(internalNode->start) << ", " << std::to_string(internalNode->end) << ") " << s.substr(internalNode->start, internalNode->end - internalNode->start + 1) << std::endl; 
            
            // Update existing node (now becomes child)
            existing->start = internalNode->end + 1;
            existing->parent = internalNode;
            std::cout << "Existing Node Modified: (" << std::to_string(existing->start) << ", " << std::to_string(existing->end) << ") " << s.substr(existing->start, existing->end - existing->start + 1) << std::endl;
            
            // Create new leaf
            Node* leaf = new Node();
            leaf->start = index + i;  // Critical fix: position in original string
            leaf->end = -1;
            leaf->suffixIndex = index;
            leaf->parent = internalNode;
            std::cout << "New leaf node created: (" << std::to_string(existing->start) << ") " << s.substr(existing->start) << std::endl;
            
            // Update pointers
            v->children[firstChar] = internalNode;
            internalNode->children[s[existing->start]] = existing;
            internalNode->children[x[i]] = leaf;
            break;
        }
        else
        {
            v = existing; 
            x = x.substr(i);
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
