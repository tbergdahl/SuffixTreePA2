#include "tree.h"
#include <iostream>

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

    // helper var - tracks number of characters in the suffix we've matched so far (in case the suffix fully matches an edge label)
    int matched = 0;

    auto x = s.substr(index);

    if (x.empty()) return;

    while (true) 
    {
        // take x[1]
        char firstChar = x[0];

        // see if an entry for the child exists in the map
        auto existing_child_entry = v->children.find(firstChar);

        // no existing child - create new leaf
        if (existing_child_entry == v->children.end()) 
        {
            Node* leaf = new Node();
            
            // make the leaf start at the point of the last matched character
            leaf->start = index + matched;
            leaf->end = -1;
            leaf->suffix_index = index;
            leaf->parent = v; 

            // see below - x may be modified if we matched an entire edge
            leaf->string_depth = v->string_depth + x.length();

            // add as child
            v->children[firstChar] = leaf;

            // keep track of what we inserted for insert_suffix()
            last_inserted_leaf = leaf;
            num_leaf_nodes++;
            break;
        }

        // child entry does exist, so search for a spot to break, or a match

        // removing some referencing
        Node* existing = existing_child_entry->second;

        // start at the child's suffix
        int suf_start = existing->start;

        // if the child is a leaf, compare against the remainder of the string
        int suf_end = (existing->end == -1) ? (s.length() - 1) : existing->end;

        // setup loop vars - j moves along s, i moves along x
        int j = suf_start;
        int i = 0;

        // Compare characters from existing node's edge & x until a mismatch occurs or one of them becomes exhausted
        while (i < x.size() && j <= suf_end && x[i] == s[j]) 
        {
            i++;
            j++;
        }

        // If x matches the entire existing node's edge, move to the child of the existing node to continue comparing 
        if (j > suf_end) 
        {
            // traverse tree
            v = existing;

            // so far, we've matched i characters. need to retain that information over the next iterations
            matched += i;

            // we've already compared i characters of x and got a full match. compare the remaining characters in the next iteration
            x = x.substr(i);
            continue;
        }

        // Mismatch found
        if (x[i] != s[j]) 
        {
            // Create internal node
            Node* internal_node = new Node();

            // new internal node starts at the same spot,
            internal_node->start = suf_start;

            // but ends at the character before the mismatch
            internal_node->end = suf_start + i - 1;

            // initialize parent & depth
            internal_node->parent = v;
            internal_node->string_depth = (internal_node->parent->string_depth + i);

            // make the existing node hold the suffix which was not common 
            // (the end of the internal node holds the index of the last matched char)
            existing->start = internal_node->end + 1;

            // make parent the node holding the common suffix (internal node)
            existing->parent = internal_node;

            // create new leaf for non-matching part of the inserted suffix
            Node* leaf = new Node();

            // adjust start to be the position of the first unmatched character, including matched characters from prev iterations
            leaf->start = index + matched + i; 
            leaf->end = -1;
            leaf->suffix_index = index;

            // make child of common suffix between inserted and existing
            leaf->parent = internal_node;
            leaf->string_depth = internal_node->string_depth + (x.size() - i);

            // insert nodes into tree
            v->children[firstChar] = internal_node;
            internal_node->children[s[existing->start]] = existing;
            internal_node->children[x[i]] = leaf;

            // track our last insert for insert_suffix
            last_inserted_leaf = leaf;

            num_internal_nodes++;
            num_leaf_nodes++;
            break;
        }
    }
}

// Inside the Tree class implementation in "tree.cpp" or your existing file

std::string Tree::computeBWT(const std::string& s) const {
    std::vector<int> suffixArray = collectSuffixIndices();
    std::string bwt;
    int n = s.length();
    
    bool has_terminator = (s.back() == '$');
    int terminator_pos = has_terminator ? n-1 : n;
    
    for (int index : suffixArray) {
        int bwtIndex = (index - 1 + n) % n;
        // Skip adding terminator if it wasn't in original string
        if (!has_terminator && bwtIndex == terminator_pos) {
            bwt.push_back('$');
        } else {
            bwt.push_back(s[bwtIndex]);
        }
    }
    return bwt;
}

void Tree::collectSuffixIndicesHelper(Node* node, std::vector<int>& indices) {
    if (node->end == -1) { 
        // Only collect if not the root and valid suffix
        if (node != root && node->suffix_index >= 0) {
            indices.push_back(node->suffix_index);
        }
        return;
    }
    // Traverse children in lex order (edges are sorted via std::map)
    for (const auto& child : node->children) {
        collectSuffixIndicesHelper(child.second, indices);
    }
}

std::vector<int> Tree::collectSuffixIndices() {
    std::vector<int> indices;
    collectSuffixIndicesHelper(root, indices);
    return indices;
}

void Tree::insert_suffix(const std::string& input, int pos)
{
    if (!last_inserted_leaf) last_inserted_leaf = root;

    Node* u = last_inserted_leaf->parent;
    if (!u) u = root;

    // cases 1A and 1B
    if (u->suffix_link)
    {
        // Follow the suffix link
        u = u->suffix_link;
        find_path(u, input, pos + u->string_depth);
    }
    else
    {
        // No suffix link, take grandparents, then hop down that subtree to find the correct spot to insert
        Node* grandparent = u->parent;

        // collapse cases 2A & 2B into these statements
        bool is_gp_root = (grandparent == root);

        // start at root if grandparent is root, else take suffix link
        Node* v   = is_gp_root ? root : grandparent->suffix_link;

        // if starting from grandparent's link, take away that string depth so we don't go too far
        int depth = is_gp_root ? 0    : grandparent->string_depth;

        // hop to the node we need to insert under
        v = node_hop(v, (u->string_depth - depth), input);

        // insert under v using find_path
        find_path(v, input, pos + v->string_depth);
        u->suffix_link = v;  // update link
    }
}

Node* Tree::node_hop(Node* n, int depth, const std::string& s)
{
    if (!n) return root;

    while (depth > 0 && n != root)
    {
        // take edge length of node
        int edge_len = n->end - n->start + 1;

        // if we have more depth to traverse than the existing edge is long, go to the node's correct child
        if (depth >= edge_len)
        {
            depth -= edge_len;
            char next_char = s[n->start + edge_len];
            auto existing_child_entry = n->children.find(next_char);
            n = existing_child_entry->second;
        }
        else // we have less depth than the existing edge. we will insert right here.
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
}
