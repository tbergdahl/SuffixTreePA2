#include "tree.h"
#include <queue>

// Public methods for tree

namespace SuffixTree
{
constexpr char ESCAPE_CHAR = '$';

Tree Tree::build(std::string const& str1, std::string const& str2, std::string const& alphabet)
{
    auto tree = Tree();
    // if any characters in the input are not a part of the alphabet, return empty tree
    if(str1.find_first_not_of(alphabet) != std::string::npos && str2.find_first_not_of(alphabet) != std::string::npos)
    {
        return tree;
    }

    std::string input = str1 + "#" + str2 + ESCAPE_CHAR;
    for (int i = 0; i < input.length(); i++) {
        tree.insert_suffix(input, i);
    }
    tree.propagate_origins(tree.root);
    return tree;
}

std::string Tree::enumerate_nodes() const {
    return enumerate(root);
}

Node * Tree::find_shared_ancestor(Node * n)
{
    Node* parent = n->parent;
    if(parent->first_string_visited && parent->second_string_visited)
    {
        return parent;
    }
    else
    {
        return find_shared_ancestor(parent);
    }
}

Node* Tree::find_deepest_shared_internal_node() 
{
    std::queue<Node*> q;
    q.push(root);
    Node *deepest = root;

    while (!q.empty())
    {
        Node *cur = q.front();

        if(cur->first_string_visited && cur->second_string_visited)
        {
            deepest = (cur->string_depth > deepest->string_depth) ? cur : deepest;
        }

        for(auto child : cur->children)
        {
            q.push(child.second);
        }

        q.pop();
    }

    return deepest;
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







}