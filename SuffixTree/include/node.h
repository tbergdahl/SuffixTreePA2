#ifndef NODE
#define NODE

#include <map>
#include <string>
#include <memory>

namespace SuffixTree
{

enum class NodeType
{
    INTERNAL,
    LEAF
};

struct Node {
    // each holds a range of a string
    int start;        // Start index of the substring
    int end;         // End index of the substring (to support extension in Ukkonen, but optional here)

    int suffixIndex;  // Leaf node's suffix index
    std::map<char, Node*> children;

    Node(int s = -1, int e = -1)
        : start(s), end(e), suffixIndex(-1) {}
};

}

#endif