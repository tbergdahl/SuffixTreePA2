#ifndef TREE
#define TREE

#include "node.h"


#include <iostream>
#include <string>
#include <vector>
#include <stack>

namespace SuffixTree
{
class Tree
{
public:
    // Create a suffix tree given an input string. Returns an empty tree if the input contains a character not in the input alphabet.
    static Tree build(std::string const& str, std::string const& alphabet);

    void display_children(Node* node) const;

    // print all nodes and their string depth
    void enumerate_nodes() const;

    // creates a bwt for the current tree
    std::string compute_BWT(const std::string& s) const;

    // returns the number of leaf nodes in the tree
    unsigned leaf_node_count() const;

    // returns the number of internal nodes in the tree
    unsigned internal_node_count() const;

    unsigned total_node_count() const;

    unsigned average_string_depth() const;

    unsigned deepest_string_depth() const;

private:
    Tree();

    // used for debugging
    void display(Node* node, std::string const& orig_str, int indent = 0);

    // find a path starting from the input node, such that the input suffix can be inserted at the end of the path
    void find_path(Node *u, std::string const& s, int i);

    // insert a suffix into the tree
    void insert_suffix(std::string const& input, int pos);

    // traverse down starting from the input node, until the input string depth is reached
    Node* node_hop(Node* n, int depth, std::string const& s);

    // tree traversal static to assist build_tree()
    static Node* last_inserted_leaf;

    // resets above static
    static void reset();

    void dfs_helper(const Node* node, std::vector<unsigned>& depths) const;

    Node* root;

    unsigned num_leaf_nodes = 0;
    unsigned num_internal_nodes = 0;

    std::string user_input;

    void collect_Suffix_Indices_Helper(Node* node, std::vector<int>& indices) const;
    std::vector<int> collect_Suffix_Indices() const;
    
    // private enumerate fn called by public enumerate
    void enumerate(const Node* node) const;

};

}

#endif