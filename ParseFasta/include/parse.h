#ifndef PARSE
#define PARSE

#include <string>

namespace ParseFasta
{
std::string parse(std::string const& filename, std::string const& alphabet_filename = "");
}

#endif