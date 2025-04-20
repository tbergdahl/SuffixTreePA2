#include "tree.h"
#include "parse.h"
#include "align.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <vector>
#include <filesystem>

using namespace SuffixTree;
using namespace ParseFasta;
using namespace Alignment;

namespace fs = std::filesystem;


int main(int argc, char *argv[])
{
    char path_char;
    #if defined(_WIN32)
        path_char = '\\';
    #else 
        path_char = '/';
    #endif

    // std::string cur_dir = fs::current_path().string();
    // std::string test_dir = path_char + std::string("tests") + path_char + std::string("TestData") +path_char ;

    // std::string fasta_filename    = cur_dir + test_dir + "Covid_Wuhan.fasta";
    // std::string alphabet_filename = cur_dir + test_dir + "DNA_alphabet.txt";

    // if(argc > 1)
    // {
    //     fasta_filename = cur_dir + test_dir + std::string(argv[1]);
    // }
    // if(argc > 2)
    // {
    //     alphabet_filename = cur_dir + test_dir + std::string(argv[2]);
    // }

    // Seq_alph_pair pair = parse(fasta_filename, alphabet_filename);
    

    // std::string output_filename = cur_dir + path_char + "output" + path_char + "outputs.txt";
    // std::ofstream output_file(output_filename);


    // if (!output_file.is_open()) {
    //     std::cerr << "Error: Unable to open file for writing: " << output_filename << std::endl;
    //     return 1;
    // }

    std::string s1 = "commonsubstring";
    std::string s2 = "com";
    auto tree = Tree::build(s1, s2, "commonsubstring");
    auto deepest = tree.find_deepest_shared_internal_node();

    auto len = deepest->end - deepest->start + 1;

    std::cout << s1.substr(deepest->string_depth, len);

    return 0;

}