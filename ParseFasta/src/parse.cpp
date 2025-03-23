#include "parse.h"


namespace ParseFasta
{

std::string const DEFAULT_ALPHABET = "ACGT";

std::string parse(std::string const& filename, std::string const& alphabet_filename) {
    std::ifstream sequencesfile(filename);
    std::ifstream alphabet_file(alphabet_filename);
    std::string result;

    // alphabet file
    if (alphabet_file.is_open()) {
        std::string alphabet_line;
        while (std::getline(alphabet_file, alphabet_line)) {
            std::string result_line;
            for (char current:alphabet_line) {
                if (current != ' ') {
                    result_line += current; 
                }
            }
            result += result_line + "\n"; 
        }
        alphabet_file.close();
    } else {
        std::cerr << "Error 69 " << alphabet_filename << std::endl;
        return "";
    }


    //fasta file
    if (sequencesfile.is_open()) {
        std::string line;
        bool is_sequence = false; 
        while (std::getline(sequencesfile, line)) {
            if (!line.empty() && line[0] == '>') {
                // this is just some files have more htan one sequence it adds a line
                if (is_sequence) {
                    result += "\n";
                }
                is_sequence = true;
                continue;
            }
            if (is_sequence) {
                result += line;
            }
        }
        sequencesfile.close();
    } else {
        std::cerr << " error 696969 "<< filename<< std::endl;
        return "";
    }

    return result;
}


}