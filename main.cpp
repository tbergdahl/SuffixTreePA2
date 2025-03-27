#include "tree.h"
#include "parse.h"

#include <filesystem>

using namespace SuffixTree;
using namespace ParseFasta;

constexpr char *TEST_DIR = "\\Tests\\TestData\\";

int main(int argc, char *argv[])
{
    std::string cur_dir = std::filesystem::current_path().string();

    // default to mississipi file
    std::string fasta_filename    = cur_dir + TEST_DIR + "s2.fas";
    std::string alphabet_filename = cur_dir + TEST_DIR + "English_alphabet.txt";

    if(argc > 1) // fasta file arg exists
    {
        fasta_filename    = cur_dir + TEST_DIR + std::string(argv[1]);
    }
    if(argc > 2) // alphabet filename exists
    {
        alphabet_filename = cur_dir + TEST_DIR + std::string(argv[2]);
    }

    Seq_alph_pair pair;
    if(alphabet_filename.empty())
    {
        pair = parse(fasta_filename);
    }
    else
    {
        pair = parse(fasta_filename, alphabet_filename);
    }

    auto tree = Tree::build(pair.second, pair.first);
    std::cout << "Leaf Count: " << std::to_string(tree.leaf_node_count()) << std::endl << "Internal Node Count: " << std::to_string(tree.internal_node_count()) << std::endl;
    return 0;
}