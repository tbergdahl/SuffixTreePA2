#include "tree.h"
#include "parse.h"

using namespace SuffixTree;
using namespace ParseFasta;

int main(int argc, char *argv[])
{
    // default to mississipi file
    std::string fasta_filename    = "s2.fas";
    std::string alphabet_filename;

    if(argc > 1) // fasta file arg exists
    {
        fasta_filename    = std::string(argv[1]);
    }
    if(argc > 2) // alphabet filename exists
    {
        alphabet_filename = std::string(argv[2]);
    }

    std::string seq = parse(fasta_filename, alphabet_filename);

    auto tree = SuffixTree::Tree::build("bananaban", "ban");
    std::cout << "Leaf Count: " << std::to_string(tree.leaf_node_count()) << std::endl << "Internal Node Count: " << std::to_string(tree.internal_node_count()) << std::endl;
    return 0;
}