#include "tree.h"
#include "parse.h"

using namespace SuffixTree;
using namespace ParseFasta;

int main(int argc, char *argv[])
{
    auto tree = SuffixTree::Tree::build("MISSISSIPPI", "MISP");
    tree.display();
    std::cout << "Leaf Count: " << std::to_string(tree.leaf_count()) << std::endl << "Internal Node Count: " << std::to_string(tree.internal_count()) << std::endl;
    return 0;
}