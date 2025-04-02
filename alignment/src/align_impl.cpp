#include "align.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

using Params = Alignment::AlignmentParams;
using Result = Alignment::AlignmentResult;


enum Direction
{
    UP,
    LEFT,
    DIAGONAL
};

typedef struct DPCell
{
    int sub_score = 0;
    int del_score = 0;
    int ins_score = 0;
};

// catch the template max() recursive fn's last call with this
template<typename T>
T max(T cur)
{
    return cur;
}

// determine max of variadic number of inputs
template<typename T, typename ... Nums>
T max(T first, T second, Nums...nums)
{
    // recursively process all arguments after the current
    return max(std::max(first, second), nums...);
}

// type alias for readability
using Columns = std::vector<DPCell>;
using Table   = std::vector<Columns>;
Table get_table(int nrows, int ncols)
{
    // nrows number of entries, with each entry having ncols
    return Table(nrows, Columns(ncols));
}

int cost_substitute(char c1, char c2, Params const& params)
{
    return (c1 == c2) ? params.match_bonus : params.mismatch_penalty;
}

// additional comparisons can be made using that last param, but will do nothing if not specified
void calc_table(Table& table, std::string const& seq1, std::string const& seq2, Params const& params, 
    const std::function<int(int)>& additional_compare = [](int n) -> int{ return n; })
{
    for(int i = 1; i < seq1.length(); i++)
    {
        for(int j = 1; j < seq2.length(); j++)
        {
            // cell above and one to the left of the current
            auto diagonal = table[i - 1][j - 1];
            table[i][j].sub_score = additional_compare(max(diagonal.sub_score, diagonal.ins_score, diagonal.del_score ) + 
                                                       cost_substitute(seq1[i], seq2[j], params));

            // cell above the current cell
            auto above = table[i - 1][j];
            table[i][j].sub_score = additional_compare(max(above.del_score + params.g, 
                                                           above.sub_score + params.h + params.g, 
                                                           above.ins_score + params.h + params.g ));

            // cell left of the current cell
            auto left = table[i][j - 1];
            table[i][j].sub_score = additional_compare(max(left.ins_score + params.g, 
                                                           left.sub_score + params.h + params.g, 
                                                           left.del_score + params.h + params.g ));
        }
    }
}

Direction decide_next(DPCell const& cell)
{
    if((cell.sub_score >= cell.del_score) && (cell.sub_score >= cell.ins_score))
    {
        return Direction::DIAGONAL;
    }
    else if (cell.del_score >= cell.ins_score)
    {
        return Direction::UP;
    }
    else
    {
        return Direction::LEFT;
    }
}


Result backtrace(Table const& table, Params const& params, std::string const& seq1, std::string const& seq2, int row, int column)
{
    auto result = Result {};
    int i = row, j = column;

    auto next = decide_next(table[i][j]);
    bool is_gap = false;

    while(i > 0 || j > 0)
    {
        switch(next)
        {
            case Direction::UP:
                if(!is_gap)
                {
                    result.opening_gap_count += 1;
                    is_gap = true;
                }
                result.s1.push_back(seq1[i - 1]);
                result.s2.push_back('-');
                result.gap_count += 1;
                i--;
                break;
            case Direction::LEFT:
                if(!is_gap)
                {
                    result.opening_gap_count += 1;
                    is_gap = true;
                }
                result.s1.push_back('-');
                result.s2.push_back(seq2[j - 1]);
                result.gap_count += 1;
                j--;
                break;
            case Direction::DIAGONAL:
                result.s1.push_back(seq1[i - 1]);
                result.s2.push_back(seq2[j - 1]);
                j--;
                i--;
                if(seq1[i] == seq2[j])
                {
                    result.match_count++;
                }
                else
                {
                    result.mismatch_count++;
                }
                is_gap = false;
                break;
        }
        next = decide_next(table[i][j]);
    }

    result.score = (result.gap_count         * params.g)           + 
                   (result.opening_gap_count * params.h)           + 
                   (result.match_count       * params.match_bonus) +
                   (result.mismatch_count    * params.mismatch_penalty);                  ;
    std::reverse(result.s1.begin(), result.s1.end());
    std::reverse(result.s2.begin(), result.s2.end());
    return result;
}


Result local_align(std::string const& seq1, std::string const& seq2, Params const& params)
{
    auto table = get_table(seq1.length() + 1, seq2.length() + 1);
    
    // extra work for local - have to init table with non-zero vals
    for (int i = 1; i < table.size(); i++)
    {   
        // init rows
        table[i][0].del_score = params.h + ((i - 1) * params.g);
    }
    for (int j = 1; j < table[0].size(); j++)
    {   
        // init cols
        table[0][j].ins_score = params.h + ((j - 1) * params.g);
    }

    // this is where the magic happens - the additional compare against zero lets me reuse the calc_table code
    // to additionally check against zero for local alignment
    calc_table(table, seq1, seq2, params, [](int n1)
    {
        return (n1 < 0) ? 0 : n1;
    });

    // now, save max position in the table and begin backtrace from there
    int max_score = 0, max_i = 0, max_j = 0;
    for (int i = 1; i < seq1.length() + 1; i++)
    {
        for (int j = 1; j < seq2.length() + 1; j++)
        {
            auto cell = table[i][j];
            int cur_max = max(cell.del_score, cell.ins_score, cell.sub_score);
            if(cur_max > max_score)
            {
                max_score = cur_max;
                max_i     = i;
                max_j     = j;
            }
        }
    }

    return backtrace(table, params, seq1, seq2, max_i, max_j);
}

Result global_align(std::string const& seq1, std::string const& seq2, Params const& params)
{
   auto table = get_table(seq1.length() + 1, seq2.length() + 1);
   calc_table(table, seq1, seq2, params);
   return backtrace(table, params, seq1, seq2, seq1.length(), seq2.length());
}

