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
    static Tree build(std::string const& str, std::string const& alphabet);

    void display_children(Node* node) const;

    void enumerate_nodes() const;

    // creates a bwt for the current tree
    std::string compute_BWT(const std::string& s);
    // returns the number of leaf nodes in the tree
    unsigned leaf_node_count() const;


    unsigned internal_node_count() const;

    unsigned total_node_count() const;

    unsigned average_string_depth() const;


    unsigned deepest_string_depth() const;

    
    unsigned find_deepest_internal_node() const;
    

private:
    Tree();

    // used for debugging
    void display(Node* node, std::string const& orig_str, int indent = 0);

    // find a path starting from the input node
    void find_path(Node *u, std::string const& s, int i);

    // insert a suffix into the tree
    void insert_suffix(std::string const& input, int pos);

    // traverse down starting from the input node
    Node* node_hop(Node* n, int depth, std::string const& s);

    // tree traversal static to help build
    static Node* last_inserted_leaf;

    // resets above static
    static void reset();

    void dfs_helper(const Node* node, std::vector<unsigned>& depths) const;

    Node* root;

    unsigned num_leaf_nodes = 0;
    unsigned num_internal_nodes = 0;

    std::string user_input; 
    void collect_Suffix_Indices_Helper(Node* node, std::vector<int>& indices);
    std::vector<int> collect_Suffix_Indices();


    
    
    void enumerate(const Node* node) const;
    
    void collect_leaf_substrings(const Node* node, std::string current_str, std::multimap<std::string, int>& substring_map) const;

};

}

#endif