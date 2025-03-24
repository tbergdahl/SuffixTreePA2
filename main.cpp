#include "tree.h"
#include "parse.h"

using namespace SuffixTree;
using namespace ParseFasta;

int main(void)
{
    auto tree = SuffixTree::Tree::build("banana", "abcdefghijklmnopqrstuvwxyz");
    tree.display();
    return 0;
}