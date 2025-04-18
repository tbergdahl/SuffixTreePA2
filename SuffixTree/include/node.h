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

enum StringOrigin { FIRST, SECOND };

struct Node 
{

    int start;
    int end;

    int suffix_index;

    bool first_string_visited = false;
    bool second_string_visited = false;

    std::map<char, Node*, AlphabetOrdering> children;

    Node* parent;

    Node *suffix_link;
    
    int string_depth;
};
}


#endif