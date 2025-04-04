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

struct DPCell
{
    int sub_score = 0;
    int del_score = 0;
    int ins_score = 0;
};

template<typename T>
T max(T cur)
{
    return cur;
}

template<typename T, typename ... TArgs>
T max(T first, T second, TArgs... args)
{
    return max(std::max(first, second), args...);
}

using Columns = std::vector<DPCell>;
using Table = std::vector<Columns>;
Table get_table(int nrows, int ncols)
{
    return Table(nrows, Columns(ncols));
}

int cost_substitute(char c1, char c2, Params const& params)
{
    return (c1 == c2) ? params.match_bonus : params.mismatch_penalty;
}

void calc_table(Table& table, std::string const& seq1, std::string const& seq2, Params const& params,
    const std::function<int(int)>& additional_compare = [](int n) -> int { return n; })
{
    for (int i = 1; i < table.size(); i++)
    {
        for (int j = 1; j < table[0].size(); j++)
        {
            DPCell& current = table[i][j];
            const DPCell& diagonal = table[i - 1][j - 1];
            current.sub_score = additional_compare(
                max(diagonal.sub_score, diagonal.del_score, diagonal.ins_score) + 
                cost_substitute(seq1[i - 1], seq2[j - 1], params)
            );

            const DPCell& above = table[i - 1][j];
            current.del_score = additional_compare(
                max(above.del_score + params.g,
                    above.sub_score + params.h + params.g,
                    above.ins_score + params.h + params.g)
            );

            const DPCell& left = table[i][j - 1];
            current.ins_score = additional_compare(
                max(left.ins_score + params.g,
                    left.sub_score + params.h + params.g,
                    left.del_score + params.h + params.g)
            );
        }
    }
}

Direction decide_next(Table const& table, int i, int j)
{
    const DPCell& current = table[i][j];
    int max_score = max(current.sub_score, current.del_score, current.ins_score);

    if (max_score == current.sub_score)
    {
        return Direction::DIAGONAL;
    }
    else if (max_score == current.del_score)
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
    Result result{"","", 0,0,0,0};
    int i = row, j = column;
    bool is_gap = false;

    while (i > 0 || j > 0)
    {
        Direction dir = decide_next(table, i, j);
        switch (dir)
        {
            case Direction::DIAGONAL:
                result.s1.push_back(seq1[i - 1]);
                result.s2.push_back(seq2[j - 1]);
                i--;
                j--;
                if (seq1[i] == seq2[j])
                {
                    result.match_count++;
                }
                else
                {
                    result.mismatch_count++;
                }
                is_gap = false;
                break;
            case Direction::UP:
                if (!is_gap)
                {
                    result.opening_gap_count++;
                    is_gap = true;
                }
                result.s1.push_back(seq1[i - 1]);
                result.s2.push_back('-');
                result.gap_count++;
                i--;
                break;
            case Direction::LEFT:
                if (!is_gap)
                {
                    result.opening_gap_count++;
                    is_gap = true;
                }
                result.s1.push_back('-');
                result.s2.push_back(seq2[j - 1]);
                result.gap_count++;
                j--;
                break;
        }
    }

    result.score = (result.opening_gap_count * params.h) +
                   (result.opening_gap_count * params.g) +
                   (result.match_count * params.match_bonus) +
                   (result.mismatch_count * params.mismatch_penalty);
    std::reverse(result.s1.begin(), result.s1.end());
    std::reverse(result.s2.begin(), result.s2.end());
    return result;
}

Result local_align(std::string const& seq1, std::string const& seq2, Params const& params)
{
    Table table = get_table(seq1.length() + 1, seq2.length() + 1);
    calc_table(table, seq1, seq2, params, [](int n) { return std::max(n, 0); });

    int max_score = 0, max_i = 0, max_j = 0;
    for (int i = 0; i < table.size(); i++)
    {
        for (int j = 0; j < table[0].size(); j++)
        {
            const DPCell& cell = table[i][j];
            int current_max = max(cell.sub_score, cell.del_score, cell.ins_score);
            if (current_max > max_score)
            {
                max_score = current_max;
                max_i = i;
                max_j = j;
            }
        }
    }

    return backtrace(table, params, seq1, seq2, max_i, max_j);
}

Result global_align(std::string const& seq1, std::string const& seq2, Params const& params)
{
    Table table = get_table(seq1.length() + 1, seq2.length() + 1);

    table[0][0].sub_score = 0;
    table[0][0].del_score = 0;
    table[0][0].ins_score = 0;

    for (int i = 1; i < table.size(); i++)
    {
        table[i][0].del_score = params.h + (i * params.g);
        table[i][0].sub_score = -1e6;
        table[i][0].ins_score = -1e6;
    }
    for (int j = 1; j < table[0].size(); j++)
    {
        table[0][j].ins_score = params.h + (j * params.g);
        table[0][j].sub_score = -1e6;
        table[0][j].del_score = -1e6;
    }

    calc_table(table, seq1, seq2, params);

    auto cell = table[seq1.length()][seq2.length()];

    std::cout << "Score: " << max(cell.del_score, cell.ins_score, cell.sub_score);

    return backtrace(table, params, seq1, seq2, seq1.length(), seq2.length());
}