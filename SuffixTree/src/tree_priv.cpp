#include "tree.h"
#include <iostream>
#include <algorithm> 
    #include <map>
namespace SuffixTree
{
Tree::Tree()
{
    root = new Node();
    root->start = root->end = 0;
    root->suffix_link = root;
    root->parent = root;
    num_internal_nodes++; // root counts :)
    reset();
}

Node* Tree::last_inserted_leaf = nullptr;

void Tree::reset()
{
    last_inserted_leaf = nullptr;
}

void Tree::find_path(Node* u, const std::string& s, int index) 
{
    Node* v = u;

  
    int matched = 0;

    auto x = s.substr(index);

    if (x.empty()) return;

    while (true) 
    {

        char firstChar = x[0];


        auto existing_child_entry = v->children.find(firstChar);

  
        if (existing_child_entry == v->children.end()) 
        {
            Node* leaf = new Node();
            

            leaf->start = index + matched;
            leaf->end = -1;
            leaf->suffix_index = index;
            leaf->parent = v; 


            leaf->string_depth = v->string_depth + x.length();


            v->children[firstChar] = leaf;


            last_inserted_leaf = leaf;
            num_leaf_nodes++;
            break;
        }




        Node* existing = existing_child_entry->second;
        int suf_start = existing->start;

        int suf_end = (existing->end == -1) ? (s.length() - 1) : existing->end;

        int j = suf_start;
        int i = 0;

        while (i < x.size() && j <= suf_end && x[i] == s[j]) 
        {
            i++;
            j++;
        }
 
        if (j > suf_end) 
        {
            // traverse tree
            v = existing;
            matched += i;

            x = x.substr(i);
            continue;
        }

        // Mismatch 
        if (x[i] != s[j]) 
        {
            Node* internal_node = new Node();
            internal_node->start = suf_start;
            internal_node->end = suf_start + i - 1;
            internal_node->parent = v;
            internal_node->string_depth = (internal_node->parent->string_depth + i);

            existing->start = internal_node->end + 1;


            existing->parent = internal_node;

            Node* leaf = new Node();

            leaf->start = index + matched + i; 
            leaf->end = -1;
            leaf->suffix_index = index;

            leaf->parent = internal_node;
            leaf->string_depth = internal_node->string_depth + (x.size() - i);

            v->children[firstChar] = internal_node;
            internal_node->children[s[existing->start]] = existing;
            internal_node->children[x[i]] = leaf;

            last_inserted_leaf = leaf;

            num_internal_nodes++;
            num_leaf_nodes++;
            break;
        }
    }
}



std::string Tree::compute_BWT(const std::string& s) {
    std::vector<int> suffixArray = collect_Suffix_Indices();
    std::string bwt;
    int n = s.length();
    
    bool has_terminator = (s.back() == '$');
    int terminator_pos = has_terminator ? n-1 : n;
    
    for (int index : suffixArray) {
        int bwtIndex = (index - 1 + n) % n;
        if (!has_terminator && bwtIndex == terminator_pos) {
            bwt.push_back('$');
        } else {
            bwt.push_back(s[bwtIndex]);
        }
    }
    return bwt;
}

void Tree::collect_Suffix_Indices_Helper(Node* node, std::vector<int>& indices) {
    if (node->end == -1) { 
        if (node != root && node->suffix_index >= 0) {
            indices.push_back(node->suffix_index);
        }
        return;
    }
    for (const auto& child : node->children) {
        collect_Suffix_Indices_Helper(child.second, indices);
    }
}

std::vector<int> Tree::collect_Suffix_Indices() {
    std::vector<int> indices;
    collect_Suffix_Indices_Helper(root, indices);
    return indices;
}

void Tree::insert_suffix(const std::string& input, int pos)
{
    if (!last_inserted_leaf) last_inserted_leaf = root;

    Node* u = last_inserted_leaf->parent;
    if (!u) u = root;

    if (u->suffix_link)
    {
        u = u->suffix_link;
        find_path(u, input, pos + u->string_depth);
    }
    else
    {

        Node* grandparent = u->parent;


        bool is_gp_root = (grandparent == root);


        Node* v   = is_gp_root ? root : grandparent->suffix_link;

        int depth = is_gp_root ? 0    : grandparent->string_depth;

        // hop to the node we need to insert under
        v = node_hop(v, (u->string_depth - depth), input);
        find_path(v, input, pos + v->string_depth);
        u->suffix_link = v;  
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
            char next_char = s[n->start + edge_len];
            auto existing_child_entry = n->children.find(next_char);
            n = existing_child_entry->second;
        }
        else 
        {
            break;
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
        std::cout << "Leaf: " << orig_str.substr(node->start) << " (Label: " << node->suffix_index << ")" << std::endl;
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


void Tree::enumerate(const Node* node) const {
    if (!node) return;
    
    std::cout << "Node at depth " << node->string_depth << ": ";
    
    if (node == root) {
        std::cout << "[ROOT]";
    } 
    else if (node->end == -1) {
        std::cout << "LEAF (suffix index: " << node->suffix_index << ")";
    } 
    else {
        std::cout << "INTERNAL (edge: [" << node->start << "," 
                  << (node->end == -1 ? "∞" : std::to_string(node->end)) << "])";
    }
    
    if (node->suffix_link && node->suffix_link != root) {
        std::cout << " → SL to depth " << node->suffix_link->string_depth;
    }
    
    std::cout << std::endl;
    for (const auto& child : node->children) {
        enumerate(child.second);
    }
}











}
