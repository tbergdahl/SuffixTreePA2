#include <string> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>

namespace ParseFasta
{
std::string parse(std::string const& filename, std::string const& alphabet_filename = "");
}