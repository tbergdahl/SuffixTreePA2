#include "parse.h"
#include <utility>



namespace ParseFasta
{
std::string const DEFAULT_ALPHABET = "ACGT";

Seq_alph_pair parse(std::string const& filename, std::string const& alphabet_filename) 
{
    std::ifstream sequences_file (filename);
    
    Seq_alph_pair output {};

    if(!alphabet_filename.empty())
    {
        std::ifstream alphabet_file(alphabet_filename);
        // alphabet file
        if (alphabet_file.is_open()) 
        {
            std::string alphabet_line;
            while (std::getline(alphabet_file, alphabet_line)) 
            {
                std::string result_line;
                for (char current : alphabet_line) 
                {
                    if (current != ' ') 
                    {
                        output.first += current; 
                    }
                }
            }
            alphabet_file.close();
        } 
        else 
        {
            throw std::exception("File did not open");
        }
    }
    else
    {
        output.first = DEFAULT_ALPHABET;
    }

    //fasta file
    if (sequences_file.is_open()) 
    {
        std::string line;
        bool is_sequence = false; 
        while (std::getline(sequences_file, line)) 
        {
            if (!line.empty() && line[0] == '>') 
            {
                // this is just some files have more htan one sequence it adds a line
                is_sequence = true;
                continue;
            }
            if (is_sequence) 
            {
                output.second += line;
            }
        }
        sequences_file.close();
    } 
    else 
    {
        throw std::exception("File did not open");
    }

    return output;
}


}