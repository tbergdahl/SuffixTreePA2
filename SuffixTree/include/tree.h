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
    static Tree build(std::string const& str, std::string const& alphabet);

    void Tree::display() {
        std::cout << "Suffix Tree:" << std::endl;
        display(root, user_input);
    }

    void enumerate_nodes() const;

    void bwt_for(std::string const& str) const;
private:
    Tree() = default;

    void display(Node* node, std::string const& orig_str, int indent = 0);

    void find_path(Node *u, std::string const& s, int i);

    void build_tree(std::string const& input, int pos);

    Node* node_hop(Node* n, int index);

    // tree traversal statics to assist build_tree()
    static Node* last_inserted_leaf;

    // resets above statics
    static void reset();


    Node* root;

    unsigned node_count;

    std::string user_input; // for testing

};

}

#endif