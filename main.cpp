#include "tree.h"
#include "parse.h"
#include "align.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <filesystem>

using namespace SuffixTree;
using namespace ParseFasta;
using namespace Alignment;

namespace fs = std::filesystem;
using SimilarityMatrix = std::vector<std::vector<int>>;

std::vector<std::string> parse_covid_files()
{
    std::vector<std::string> sequences;
    const std::filesystem::path test_dir = std::filesystem::current_path() / "tests" / "TestData2";

    for (const auto& eachEntry : std::filesystem::directory_iterator(test_dir)) {
        if (eachEntry.path().extension() == ".fasta") {
            try {
                auto sequencePair = ParseFasta::parse(eachEntry.path().string(), "");
                sequences.push_back(sequencePair.second);
            } catch (const std::exception& e) {
                std::cerr << "error bruh " << std::endl;
            }
        }
    }

    return sequences;
}


// used to find the start coordinates of common substring on s1
int find_leaf_suffix_index(Node* node) {
    if (node->children.empty()) {
        // This is a leaf, return its suffix index
        return node->suffix_index;
    }

    for (auto& child : node->children) {
        if (child.second->first_string_visited) {
            return find_leaf_suffix_index(child.second);
        }
    }

    return -1; // Shouldn't happen if the tree is valid
}

void print_matrix(std::ostream& dest, SimilarityMatrix const& matrix)
{
    dest << std::setw(10) << " ";
    for (size_t j = 0; j < matrix.size(); ++j) {
        dest << std::setw(10) << j+1;
    }
    dest << "\n";

    for (size_t i = 0; i < matrix.size(); ++i) {
        dest << std::setw(10) << i+1;
        for (size_t j = 0; j < matrix.size(); ++j) {
            if (j <= i) {
                dest << std::setw(10) << " ";
            } else {
                dest << std::setw(10) << matrix[i][j];
            }
        }
        dest << "\n";
    }
}

int compute_similarity(std::string const& s1, std::string const& s2, std::string const& alphabet)
{
    auto tree = Tree::build(s1, s2, alphabet);

    /*
        Begin calculating a, b, c from assignment description
    */
    int a, b, c;

    Node* deepest = tree.find_deepest_shared_internal_node();

    int start_index = find_leaf_suffix_index(deepest);
    int length = deepest->string_depth;
    
    //std::cout << s1.substr(start_index, length) << std::endl;

    // extract common substring
    std::string substr = s1.substr(start_index, length);

    // make sure it exists in s2
    auto pos = s2.find_first_of(substr);
    if(pos == std::string::npos)
    {
        throw std::runtime_error("UGH");
    }

    {
        /*
            Compute B from assignment description
        */
        b = substr.length();
    }

    AlignmentParams params { 1, -2, -5, -1 };
    // calculate the coordinates of the sub-sections of the strings to align
    size_t x_1 = start_index, x_2 = pos;
    size_t y_1 = start_index + length, y_2 = pos + length;
    {
        /*
            Compute A from assignment description
        */
        std::string s1_prefix = s1.substr(0, x_1);
        std::string s2_prefix = s2.substr(0, x_2);

        std::string s_1_rev, s_2_rev;
        std::reverse_copy(s1_prefix.begin(), s1_prefix.end(), std::back_inserter(s_1_rev));
        std::reverse_copy(s2_prefix.begin(), s2_prefix.end(), std::back_inserter(s_2_rev));
        a = Alignment::get_match_count(s_1_rev, s_2_rev, params);
    }

    {
        /*
            Compute c from assignment description
        */
        auto s_1_fwd = s1.substr(y_1);
        auto s_2_fwd = s2.substr(y_2);

        c = Alignment::get_match_count(s_1_fwd, s_2_fwd, params);
    }
    return (a + b + c);
}


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
    
    /*
    
    auto sequence = parse_covid_files();

    for (size_t i = 0; i < sequence.size(); ++i) {
        std::cout << "Sequence " << i + 1 << ": " << sequence[i] << "\n";
    }
    */
    auto sequences = parse_covid_files();
    auto matrix = SimilarityMatrix(sequences.size(), std::vector<int>(sequences.size()));
    for(int i = 0; i < sequences.size(); i++)
    {
        for(int j = i + 1; j < sequences.size(); j++)
        {
            matrix[i][j] = compute_similarity(sequences[i], sequences[j], "AGCT");
        }
    }
    
    print_matrix(std::cout, matrix);

    return 0;

}