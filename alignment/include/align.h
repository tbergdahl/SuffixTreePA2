#include <string>

namespace Alignment
{

enum class AlignmentType
{
    GLOBAL,
    LOCAL
};

struct AlignmentParams
{
    int match_bonus;
    int mismatch_penalty;
    int h; // consecutive gap penalty
    int g; // initial gap penalty
};

void align(std::string const& seq1, std::string const& seq2, AlignmentType const& type, AlignmentParams const& params);

}