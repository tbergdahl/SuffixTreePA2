#ifndef TREE
#define TREE

#include "node.h"


#include <iostream>
#include <string>

namespace SuffixTree
{
class Tree
{
public:
    // Create a suffix tree given an input string. Returns an empty tree if the input contains a character not in the input alphabet.
    static Tree build(std::string const& str, std::string const& alphabet);

    // not sure
    void enumerate_nodes() const;

    // creates a bwt for the current tree
    void bwt_for(std::string const& str) const;

    // returns the number of leaf nodes in the tree
    unsigned leaf_node_count() const
    {
        return num_leaf_nodes;
    }

    // returns the number of internal nodes in the tree
    unsigned internal_node_count() const
    {
        return num_internal_nodes;
    }
    std::string computeBWT(const std::string& s);
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


    Node* root;

    unsigned num_leaf_nodes = 0;

    std::string user_input; // for testing
    void collectSuffixIndicesHelper(Node* node, std::vector<int>& indices);
    std::vector<int> collectSuffixIndices();
};

}

#endif