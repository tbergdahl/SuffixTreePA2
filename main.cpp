#include "tree.h"
#include "parse.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <vector>
#include <filesystem>

using namespace SuffixTree;
using namespace ParseFasta;

constexpr char *TEST_DIR = "\\Tests\\TestData\\";


namespace fs = std::filesystem;

void measure_construction_performance(const std::string& test_dir) {
    // Header for performance report
    std::cout << "\nSuffix Tree Construction Performance Report\n";
    std::cout << "==========================================\n";
    std::cout << "| File Name               | Length (bp) | Time (ms) | Memory (KB) | Nodes |\n";
    std::cout << "|-------------------------|-------------|-----------|-------------|-------|\n";

    // Process each FASTA file in test directory
    for (const auto& entry : fs::directory_iterator(test_dir)) {
        if (entry.is_regular_file() && 
            entry.path().extension() == ".fasta" && 
            entry.path().string().find(".BWT.") == std::string::npos) {
            
            try {
                // Read FASTA file
                std::ifstream input_file(entry.path());
                //std::string sequence = ParseFasta::read_sequence(input_file) + "$"; // Add terminator
                input_file.close();

                // Get alphabet (simplified - adjust as needed)
                std::string alphabet = "ACGT$"; // Default for DNA
                if (entry.path().stem().string().find("English") != std::string::npos) {
                    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ$";
                }

                // Measure construction
                auto start = std::chrono::high_resolution_clock::now();
                // auto tree = SuffixTree::Tree::build(sequence, alphabet);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                // Get memory usage (Linux-specific example)
                long mem_usage = 0;
                #ifdef __linux__
                    std::ifstream statm("/proc/self/statm");
                    if (statm) {
                        statm >> mem_usage;  // Get total program pages
                        mem_usage *= sysconf(_SC_PAGESIZE) / 1024;  // Convert to KB
                    }
                #endif

                // Print results
                std::cout << "| " << std::setw(23) << entry.path().filename().string() 
                          //<< " | " << std::setw(11) << sequence.length() - 1  // Exclude $
                          << " | " << std::setw(9) << duration.count()
                          << " | " << std::setw(11) << mem_usage
                         //<< " | " << std::setw(5) << (tree.leaf_count() + tree.internal_count())
                          << " |\n";

            } catch (const std::exception& e) {
                std::cerr << "Error processing " << entry.path() << ": " << e.what() << "\n";
            }
        }
    }
}



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