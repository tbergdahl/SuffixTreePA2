#include "tree.h"
#include "parse.h"
#include "align.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace SuffixTree;
using namespace ParseFasta;
using namespace Alignment;

namespace fs = std::filesystem;
using SimilarityMatrix = std::vector<std::vector<int>>;

// Timing wrapper structure
struct ComparisonTiming {
    double suffix_tree_time = 0;
    double alignment_time = 0;
    double total_time = 0;
};

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

int compute_similarity(std::string const& s1, std::string const& s2, std::string const& alphabet, int& lcs_len, ComparisonTiming& timing)
{
    auto start_tree = std::chrono::high_resolution_clock::now();
    auto tree = Tree::build(s1, s2, alphabet);
    auto end_tree = std::chrono::high_resolution_clock::now();
    timing.suffix_tree_time = std::chrono::duration<double>(end_tree - start_tree).count();

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
        lcs_len = b = substr.length();
    }

    AlignmentParams params { 1, -2, -5, -1 };
    // calculate the coordinates of the sub-sections of the strings to align
    size_t x_1 = start_index, x_2 = pos;
    size_t y_1 = start_index + length, y_2 = pos + length;
    auto start_align = std::chrono::high_resolution_clock::now();
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
    auto end_align = std::chrono::high_resolution_clock::now();
    timing.alignment_time = std::chrono::duration<double>(end_align - start_align).count();

    timing.total_time = timing.alignment_time + timing.suffix_tree_time;

    return (a + b + c);
}


int main()
{
    try {
        std::cout << "Loading sequences...\n";
        auto sequences = parse_covid_files();
        SimilarityMatrix lcs_lengths(sequences.size(), std::vector<int>(sequences.size()));
        SimilarityMatrix matrix(sequences.size(), std::vector<int>(sequences.size()));
        std::vector<std::tuple<int, int, double, double, double>> timing_data;

        for (size_t i = 0; i < sequences.size(); ++i) {
            std::cout << "Processing Sequence " << i+1 << "\n";
            for (size_t j = 0; j < sequences.size(); ++j) {
                int similarity_score;
                int lcs_length;
                ComparisonTiming timing{ 0, 0, 0 };
                matrix[i][j] = similarity_score;
                lcs_lengths[i][j] = lcs_length;
                timing_data.emplace_back(i+1, j+1, timing.suffix_tree_time, 
                                       timing.alignment_time, timing.total_time);
                
                std::cout << "  vs Sequence " << j+1 << ": "
                          << "Tree=" << timing.suffix_tree_time << "s, "
                          << "Align=" << timing.alignment_time << "s, "
                          << "Total=" << timing.total_time << "s\n";
            }
        }

        print_matrix(std::cout, matrix);

        // Performance summary
        std::cout << "\nPerformance Summary:\n";
        std::cout << "============================================\n";
        std::cout << "Pair\tTree Time\tAlign Time\tTotal Time\n";
        std::cout << "--------------------------------------------\n";
        
        double total_tree = 0, total_align = 0, total_all = 0;
        for (const auto& [i, j, tree, align, total] : timing_data) {
            std::cout << i << "v" << j << "\t"
                      << std::fixed << std::setprecision(4) << tree << "\t\t"
                      << align << "\t\t" << total << "\n";
            total_tree += tree;
            total_align += align;
            total_all += total;
        }
        
        std::cout << "--------------------------------------------\n";
        std::cout << "TOTAL\t" << total_tree << "\t\t" << total_align << "\t\t" << total_all << "\n";
        std::cout << "AVG\t" << total_tree/timing_data.size() << "\t\t"
                  << total_align/timing_data.size() << "\t\t"
                  << total_all/timing_data.size() << "\n";
        std::cout << "============================================\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}