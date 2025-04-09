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

std::string Tree::enumerate_nodes() const {
    return enumerate(root);
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

// 
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

        if (current->string_depth > max_depth) {
            max_depth = current->string_depth;
        }
        
        // Push children onto stack 
        for (const auto& child : current->children) {
            nodes.push(child.second);
        }
    }
    
    return max_depth;
}

void Tree::display_children(Node* node) const
{
    if(node)
    {
        for (auto const& it : node->children)
        {
            Node* child = it.second;
            std::cout << "Child (start index: " << std::to_string(child->start) << ", end index: " << std::to_string(child->end) << ") = " << user_input.substr(child->start, (child->end - child->start + 1)) << std::endl
            << "String Depth: " << std::to_string(child->string_depth) << std::endl;
        }
    }
}


unsigned Tree::find_deepest_internal_node() const {
    unsigned max_depth = 0;
    std::stack<Node*> stack;
    stack.push(root);
    
    while (!stack.empty()) {
        Node* current = stack.top();
        stack.pop();
        // Check if the current node is an internal node
        // meaing not a leaf and roots
        if (current != root && current->end != -1) {
            if (current->string_depth > max_depth) {
                max_depth = current->string_depth;
            }
        }
        for (const auto& child : current->children) {
            stack.push(child.second);
        }
    }
    
    return max_depth;
}






}