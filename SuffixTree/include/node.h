#ifndef NODE
#define NODE

#include <map>
#include <string>
#include <memory>

namespace SuffixTree
{

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

    int start;
    int end;

    int suffix_index;


    std::map<char, Node*, AlphabetOrdering> children;

    Node* parent;

    Node *suffix_link;
    
    int string_depth;
};
}


#endif