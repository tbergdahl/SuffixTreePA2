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

    // Close the output file
    outfile.close();
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
    std::string test_dir = path_char + std::string("Tests") + path_char + std::string("TestData") + path_char;

    //measure_construction_performance(cur_dir + test_dir);

    // default to mississipi file
    std::string fasta_filename    = cur_dir + test_dir + "Covid_Brazil.fasta";
    std::string alphabet_filename = cur_dir + test_dir + "English_alphabet.txt";

    
    if(argc > 1) // fasta file arg exists
    {
        fasta_filename    = cur_dir + test_dir + std::string(argv[1]);
    }
    if(argc > 2) // alphabet filename exists
    {
        alphabet_filename = cur_dir + test_dir + std::string(argv[2]);
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

    std::cout << pair.first;
    std::cout << pair.second;

    

    auto tree = Tree::build(pair.second, pair.first);
    std::cout << "Leaf Count: " << std::to_string(tree.leaf_node_count()) << std::endl << "Internal Node Count: " << std::to_string(tree.internal_node_count()) << std::endl;
    std::cout << "average string depth: " << std::to_string(tree.average_string_depth()) << std::endl;
    std::cout << "Deepest string depth: " << std::to_string(tree.deepest_string_depth()) << std::endl;
    tree.enumerate_nodes();



    //std::string terminated_reference = pair.second + "$";
    //std::string bwt = tree.compute_BWT(terminated_reference);
    //std::cout << "\nBWT for \"" << bwt << std::endl;
  
    
    


    return 0;

}