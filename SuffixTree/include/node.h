#ifndef NODE
#define NODE

#include <map>
#include <string>
#include <memory>

namespace SuffixTree
{
struct Node {
    // each holds a range of a string
    int start;       // Start index of the substring
    int end;         // End index of the substring

    int suffixIndex;  // Leaf node's suffix index
    std::map<char, Node*> children;
    std::uint32_t id;
    Node* parent;
    Node *suffix_link;
    int string_depth;
};

}

#endif