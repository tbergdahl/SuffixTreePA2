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

class Node
{
public:
    Node(unsigned n_id);

    void print();

private:
    int id = -1;
    std::map<char, std::unique_ptr<Node>> children;

    NodeType type;

    friend class Tree;
};

}

#endif