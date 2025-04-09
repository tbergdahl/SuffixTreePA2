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





void measure_construction_performance(const std::string& test_dir) 
{
    std::ofstream outfile("performance_metrics.txt");

    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing\n";
        return;
    }

    outfile << "\nSuffix Tree Construction Performance Report\n";
    outfile << "========================================================================\n";
    outfile << "| File Name                          | Length (bp) | Time (ms) | Nodes |\n";
    outfile << "|------------------------------------|-------------|-----------|-------|\n";

    std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (const auto& entry : fs::directory_iterator(test_dir)) 
    {
        if (entry.is_regular_file() && (entry.path().extension() == ".fas" || entry.path().extension() == ".fasta")) {
            std::string fasta_filename = entry.path().string();

            Seq_alph_pair pair = parse(fasta_filename);

            auto start_time = std::chrono::high_resolution_clock::now();
            
            auto tree = Tree::build(pair.second, alphabet);

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

            
            size_t sequence_length = pair.second.length();

            
            size_t leaf_nodes = tree.leaf_node_count();
            size_t internal_nodes = tree.internal_node_count();
            size_t total_nodes = leaf_nodes + internal_nodes;

            
            outfile << "| " << std::setw(23) << entry.path().filename().string()
                    << " | " << std::setw(11) << sequence_length
                    << " | " << std::setw(9) << duration
                    << " | " << std::setw(5) << total_nodes
                    << " |\n";
        }
    }

    outfile << "========================================================================\n";

    outfile.close();
}



void printAlignedSequences(const std::string& seq1, const std::string& seq2, const std::string& aligned_seq1, const std::string& aligned_seq2) {
    // Print the sequence information
    std::cout << "Sequence 1 = \"" << seq1 << "\", length = " << seq1.length() << " characters\n";
    std::cout << "Sequence 2 = \"" << seq2 << "\", length = " << seq2.length() << " characters\n";

    // Calculate and print the aligned sequences in chunks
    for (size_t i = 0; i < aligned_seq1.length(); i += 60) {
        // Print 60 characters at a time
        std::cout << "s1 " << i + 1 << " " << aligned_seq1.substr(i, 60) << std::endl;
        std::cout << "s2 " << i + 1 << " " << aligned_seq2.substr(i, 60) << std::endl;

        // Print the middle line with | for matches and spaces for mismatches
        std::string match_line = "";
        for (size_t j = i; j < std::min(i + 60, aligned_seq1.length()); ++j) {
            if (aligned_seq1[j] == aligned_seq2[j] && aligned_seq1[j] != '-') {
                match_line += '|';  // Match
            } else {
                match_line += ' ';  // Mismatch or gap
            }
        }
        std::cout << match_line << std::endl;
    }
}

int main(int argc, char *argv[])
{
    char path_char;
    #if defined(_WIN32)
        path_char = '\\';
    #else 
        path_char = '/';
    #endif

    std::string cur_dir = fs::current_path().string();
    std::string test_dir = path_char + std::string("tests") + path_char + std::string("TestData") +path_char ;

    std::string fasta_filename    = cur_dir + test_dir + "Covid_Wuhan.fasta";
    std::string alphabet_filename = cur_dir + test_dir + "DNA_alphabet.txt";

    if(argc > 1)
    {
        fasta_filename = cur_dir + test_dir + std::string(argv[1]);
    }
    if(argc > 2)
    {
        alphabet_filename = cur_dir + test_dir + std::string(argv[2]);
    }

    Seq_alph_pair pair = parse(fasta_filename, alphabet_filename);
    

    std::string output_filename = cur_dir + path_char + "output" + path_char + "outputs.txt";
    std::ofstream output_file(output_filename);


    if (!output_file.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << output_filename << std::endl;
        return 1;
    }

    auto tree = Tree::build(pair.second, pair.first);
    std::string terminated_reference = pair.second + "$";
    std::string bwt2 = tree.compute_BWT(terminated_reference);
    output_file << "Leaf Count: " << std::to_string(tree.leaf_node_count()) << std::endl << "Internal Node Count: " << std::to_string(tree.internal_node_count()) << std::endl;
    output_file << "average string depth: " << std::to_string(tree.average_string_depth()) << std::endl;
    output_file << "Deepest string depth: " << std::to_string(tree.deepest_string_depth()) << std::endl;
    output_file << "Exact matching repeat: " << std::to_string(tree.find_deepest_internal_node()) << std::endl;
    output_file << tree.enumerate_nodes() << std::endl;


    

    for (char c : bwt2) {
        output_file << c << "\n";


    }

    output_file.close();
   

    //measure_construction_performance( cur_dir + test_dir);
    std::cout << "Leaf Count: " << std::to_string(tree.leaf_node_count()) << std::endl << "Internal Node Count: " << std::to_string(tree.internal_node_count()) << std::endl;
    std::cout << "average string depth: " << std::to_string(tree.average_string_depth()) << std::endl;
    std::cout << "Deepest string depth: " << std::to_string(tree.deepest_string_depth()) << std::endl;
    std::cout << "Exact matching repeat: " << std::to_string(tree.find_deepest_internal_node()) << std::endl;
    //tree.enumerate_nodes();
   
    // std::string s1 = "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT";
    // std::string s2 = "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT";
    // AlignmentParams params;
    // params.match_bonus = 1;
    // params.mismatch_penalty = -2;
    // params.h = -5;
    // params.g = -2;

    // auto res = align(s1, s2, Alignment::AlignmentType::GLOBAL, params);

    // std::cout << "Score: " << res.score << std::endl;
    // std::cout << "# of Matches: " << res.match_count << std::endl;
    // std::cout << "# of Mismatches: " << res.mismatch_count << std::endl;
    // std::cout << "# of Gaps: " << res.gap_count << std::endl;
    // std::cout << "# of Opening Gaps: " << res.opening_gap_count << std::endl;

    // printAlignedSequences(s1, s2, res.s1, res.s2);


    return 0;

}