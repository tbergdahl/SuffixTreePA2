#ifndef NODE
#define NODE

#include <map>
#include <string>
#include <memory>

namespace SuffixTree
{
// plugin to the node's children maps to define ordering of characters in the map (i.e. map '$' to smallest lexographically)
struct AlphabetOrdering
{
    bool operator()(char a, char b) const
    {
        if(a == '$' && b != '$')
        {
            return true;
        }
        if(b == '$' && a != '$')
        {
            return false;
        }
        return (a < b);
    }
};

struct Node 
{
    // the start and end indices of the substring the node holds
    int start;
    int end;

    // leaf node's suffix index
    int suffix_index;

    // children of this node
    std::map<char, Node*, AlphabetOrdering> children;

    // parent of this node
    Node* parent;

    // suffix link of this node
    Node *suffix_link;

    // string depth of this node
    int string_depth;
};
}

#endif