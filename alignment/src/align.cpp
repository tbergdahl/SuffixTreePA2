#include "align.h"

#include <iostream>
#include <vector>


using Params = Alignment::AlignmentParams;
using Result = Alignment::AlignmentResult;

Result local_align (std::string const& seq1, std::string const& seq2, Params const& params);
Result global_align(std::string const& seq1, std::string const& seq2, Params const& params);

namespace Alignment
{
// public alignment method
Result align(std::string const& seq1, std::string const& seq2, AlignmentType const& type, AlignmentParams const& params)
{
    switch(type)
    {
        case AlignmentType::LOCAL:
            return local_align (seq1, seq2, params);
        case AlignmentType::GLOBAL:
            return global_align(seq1, seq2, params);
        default:
            throw std::runtime_error("Non-implemented alginment type!");
    }
}

}