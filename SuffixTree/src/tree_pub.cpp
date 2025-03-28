#include "tree.h"

// Public methods for tree

namespace SuffixTree
{
constexpr char ESCAPE_CHAR = '$';

Tree Tree::build(std::string const& str, std::string const& alphabet)
{
    auto tree = Tree();
    // if any characters in the input are not a part of the alphabet, return empty tree
    if(str.find_first_not_of(alphabet) != std::string::npos)
    {
        return tree;
    }

    std::string input = str + ESCAPE_CHAR;
    for (int i = 0; i < input.length(); i++) {
        tree.insert_suffix(input, i);
    }

    return tree;
}

void Tree::enumerate_nodes() const
{
    enumerate(root);
}

unsigned Tree::leaf_node_count() const
{
    return num_leaf_nodes;
}

// returns the number of internal nodes in the tree
unsigned Tree::internal_node_count() const
{
    return num_internal_nodes;
}

unsigned Tree::total_node_count() const
{
    return leaf_node_count() + internal_node_count();
}

unsigned Tree::average_string_depth() const
{
    if (internal_node_count() == 0) {
        return 0;
    }
    
    unsigned total_depth = 0;
    unsigned node_count = 0;
    
    // Use a stack for iterative traversal
    std::stack<Node*> nodes;
    nodes.push(root);
    
    while (!nodes.empty()) {
        Node* current = nodes.top();
        nodes.pop();
        
        // Only count internal nodes (non-root nodes with children)
        if (!current->children.empty()) {
            total_depth += current->string_depth;
            node_count++;
        }
        
        // Push children onto stack in reverse order to process them in order
        for (auto it = current->children.rbegin(); it != current->children.rend(); ++it) {
            nodes.push(it->second);
        }
    }
    
    return total_depth / node_count;
}

unsigned Tree::deepest_string_depth() const
{
    unsigned max_depth = 0;
    
    // Use a stack for iterative traversal
    std::stack<Node*> nodes;
    nodes.push(root);
    
    while (!nodes.empty()) {
        Node* current = nodes.top();
        nodes.pop();
        
        // Update max_depth if current node's depth is greater
        if (current->string_depth > max_depth) {
            max_depth = current->string_depth;
        }
        
        // Push children onto stack to process them
        for (const auto& child : current->children) {
            nodes.push(child.second);
        }
    }
    
    return max_depth;
}

// display child 

// BWT_index

// exact matching repeat to the report 

}