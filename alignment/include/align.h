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

struct AlignmentResult
{
    std::string s1;
    std::string s2;
    int gap_count;
    int opening_gap_count;
    int match_count;
    int mismatch_count;
    int score;
};

AlignmentResult align(std::string const& seq1, std::string const& seq2, AlignmentType const& type, AlignmentParams const& params);

int get_match_count(std::string const& seq1, std::string const& seq2, AlignmentParams const& params);

}