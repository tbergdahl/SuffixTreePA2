#ifndef NODE
#define NODE

#include <map>
#include <string>
#include <memory>

namespace SuffixTree
{
struct Node 
{
    // the start and end indices of the substring the node holds
    int start;
    int end;

    // leaf node's suffix index
    int suffix_index;

    // children of this node
    std::map<char, Node*> children;

    // parent of this node
    Node* parent;

    // suffix link of this node
    Node *suffix_link;

    // string depth of this node
    int string_depth;
};
}

#endif