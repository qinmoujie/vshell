#include "line_parser.h"
#include <stack>
namespace vshell
{

using namespace std;

LineParser::LineParser() { __init_delimiter_set(); };

void LineParser::__init_delimiter_set()
{
    binary_delimiter_v.push_back(make_pair("\"", "\""));
    binary_delimiter_v.push_back(make_pair("'", "'"));
    binary_delimiter_v.push_back(make_pair("`", "`"));
    binary_delimiter_v.push_back(make_pair("[[", "]]"));
    binary_delimiter_v.push_back(make_pair("[", "]"));
    binary_delimiter_v.push_back(make_pair("$((", "))"));
    binary_delimiter_v.push_back(make_pair("$(", ")"));
    binary_delimiter_v.push_back(make_pair("((", "))"));
    binary_delimiter_v.push_back(make_pair("(", ")"));
    binary_delimiter_v.push_back(make_pair("${", "}"));
    binary_delimiter_v.push_back(make_pair("{", "}"));
    unary_delimiter_set.insert(';');
    //  unary_delimiter_set.insert('=');
}

bool LineParser::is_unary_delimiter(const char word) const
{
    return (isspace(word) || unary_delimiter_set.find(word) != unary_delimiter_set.end());
}

size_t LineParser::find_unary_delimiter(const std::string &line,
                                        size_t start, size_t end,
                                        bool is_unary) const
{
    for (; start != end && is_unary_delimiter(line[start]) != is_unary; ++start)
        ;

    return start;
}

bool LineParser::is_binary_delimiter_start(const std::vector<std::string> &inf,
                                           const size_t line_index, const size_t column_index,
                                           delimiter_pair_type &delimiter_pair)
{
    for (const delimiter_pair_type &dp : this->binary_delimiter_v)
    {
        if (dp.first.size() + line_index <= inf[line_index].size())
        {
            if (inf[line_index].substr(column_index, dp.first.size()) == dp.first)
            {
                delimiter_pair = dp;
                return true;
            }
        }
    }
    return false;
}

bool LineParser::line_parser(const std::vector<std::string> &inf,
                             std::vector<line_word_type> &ouf)
{

    stack<std::pair<delimiter_pair_type, point_type>> delimiter_stk;
    size_t line_index = 0, column_index = 0;
    delimiter_pair_type delimiter_pair;
    for (;;)
    {
        if (delimiter_stk.empty())
        {
            for (;;)
            {
                if (column_index == inf[line_index].size())
                {
                    column_index = 0;
                    line_index += 1;
                }
                if (line_index >= inf.size())
                    return true;
                if (is_binary_delimiter_start(inf, line_index, column_index, delimiter_pair))
                {
                    line_index += delimiter_pair.first.size();
                    delimiter_stk.push(make_pair(std::move(delimiter_pair), std::move(make_pair(line_index, column_index))));
                    break;
                }
            }
        }
        else
        {
            if (delimiter_stk.top().first.first == "\"")
            {
                for (;;)
                {
                    if (column_index == inf[line_index].size())
                    {
                        column_index = 0;
                        line_index += 1;
                    }
                    if (line_index >= inf.size())
                    {
                        return false;
                    }
                    if (inf[line_index][column_index] == '"')
                    {
                        line_index += 1;
                        delimiter_stk.pop();
                        break;
                    }
                }
            }
            else if (delimiter_stk.top().first.first == "\'")
            {
                for (;;)
                {
                    if (column_index == inf[line_index].size())
                    {
                        column_index = 0;
                        line_index += 1;
                    }
                    if (line_index >= inf.size())
                    {
                        return false;
                    }
                    if (inf[line_index][column_index] == '\'')
                    {
                        line_index += 1;
                        delimiter_stk.pop();
                        break;
                    }
                }
            }
            else
            {
            }
        }
    }
}

} // namespace vshell