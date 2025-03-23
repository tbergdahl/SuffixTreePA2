#ifndef NODE
#define NODE

namespace SuffixTree
{

class Node
{
public:
    Node(unsigned n_id);

    void print();

private:
    unsigned id;
};

}

#endif