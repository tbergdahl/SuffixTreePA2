#include "tree.h"
#include <iostream>

namespace SuffixTree
{
    Node * Tree::last_inserted_leaf = nullptr;

    void Tree::reset()
    {
        last_inserted_leaf = nullptr;
    }

    void Tree::find_path(Node* u, const std::string& s, int index) 
    {
        Node* v = u;
        auto x = s.substr(index);
        if (x.empty()) return;

        while (true)
        {
            char firstChar = x[0];
            auto it = v->children.find(firstChar);
            if (it == v->children.end()) 
            {
                Node* leaf = new Node();
                leaf->start = index;
                leaf->end = -1;
                leaf->suffixIndex = index;
                leaf->parent = v;
                v->children[firstChar] = leaf;
                last_inserted_leaf = leaf;
                break;
            }

            Node* existing = it->second;
            int suf_start = existing->start;
            int suf_end = (existing->end == -1 ? (s.length() - 1) : existing->end);
            int j = suf_start;
            int i = index;
            while (i < s.length() && j <= suf_end && s[i] == s[j]) i++, j++;

            bool is_mismatch = (i < s.length() && j <= suf_end && s[i] != s[j]);
            if (is_mismatch) 
            {
                Node* internalNode = new Node();
                internalNode->start = suf_start;
                internalNode->end = j - 1;
                internalNode->parent = v;
                internalNode->string_depth = v->string_depth + (j - suf_start);
                v->children[firstChar] = internalNode;

                Node* leaf = new Node();
                leaf->start = i;
                leaf->end = -1;
                leaf->suffixIndex = index;
                leaf->parent = internalNode;
                last_inserted_leaf = leaf;

                existing->start = j;
                existing->parent = internalNode;
                existing->string_depth = internalNode->string_depth + (suf_end - j + 1);

                internalNode->children[s[j]] = existing;
                internalNode->children[s[i]] = leaf;
                break;
            } 
            else 
            {
                v = existing;
            }
        }
    }

    void Tree::build_tree(std::string const& input, int pos)
    {
        if (!last_inserted_leaf) last_inserted_leaf = root;
        Node* u = last_inserted_leaf->parent;
        if (u->suffix_link)
        {
            u = u->suffix_link;
            find_path(u, input, pos + u->string_depth);
        }
        else
        {
            Node* grandparent = u->parent;
            int suffixStart = (grandparent->end == -1) ? pos : pos + grandparent->end;
            find_path(grandparent, input, suffixStart);
        }
    }

    Node* Tree::node_hop(Node* n, int depth)
    {
        return nullptr;
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
