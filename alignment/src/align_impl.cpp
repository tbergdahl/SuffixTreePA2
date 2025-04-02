#include "align.h"

#include <vector>
#include <iostream>

using Params = Alignment::AlignmentParams;

typedef struct DPCell
{
    int sub_score = 0;
    int del_score = 0;
    int ins_score = 0;
};

using Columns = std::vector<DPCell>;
using Table   = std::vector<Columns>;
Table get_table(int nrows, int ncols)
{
    // nrows number of entries, with each entry having ncols
    return Table(nrows, Columns(ncols));
}

void local_align(std::string const& seq1, std::string const& seq2, Params const& params)
{
    auto table = get_table(seq1.length() + 1, seq2.length() + 1);
    std::cout << "Hello from local!";

}

void global_align(std::string const& seq1, std::string const& seq2, Params const& params)
{
    auto table = get_table(seq1.length() + 1, seq2.length() + 1);
    std::cout << "Hello from global!";

}

