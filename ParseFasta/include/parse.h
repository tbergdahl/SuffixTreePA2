#ifndef PARSE
#define PARSE

#include <string> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <tuple>
#include <utility>

namespace ParseFasta
{
using Seq_alph_pair = std::pair<std::string, std::string>;

Seq_alph_pair parse(std::string const& filename, std::string const& alphabet_filename = "");
}

#endif