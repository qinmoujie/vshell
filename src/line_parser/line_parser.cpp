#include "line_parser.h"

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
    binary_delimiter_v.push_back(make_pair("((", "))"));
    //binary_delimiter_v.push_back(make_pair("(", ")"));
    binary_delimiter_v.push_back(make_pair("$(", ")"));
    binary_delimiter_v.push_back(make_pair("${", "}"));
    unary_delimiter_set.insert(';');
    unary_delimiter_set.insert('=');
}

bool LineParser::is_unary_delimiter(const char word) const
{
    if (isspace(word))
        return true;
    return unary_delimiter_set.find(word) != unary_delimiter_set.end();
}

size_t LineParser::find_unary_delimiter(const std::string &line,
                                        size_t start, size_t end,
                                        bool is_unary) const
{
    for (; start != end && is_unary_delimiter(line[start]) != is_unary; ++start)
        ;

    return start;
}

bool LineParser::parser_unary_delimiter_words(line_word_type &line_words,
                                              const std::string &line,
                                              size_t start, size_t end) const
{
    for (size_t pos = start; pos != end;)
    {
        start = find_unary_delimiter(line, pos, end, true);
        if (start != pos)
            line_words.emplace_back(line.substr(pos, start - pos), UNARY_DELIMITER,
                                    delimiter_pair_type("", ""));
        pos = find_unary_delimiter(line, start, end, false);
    }
    return true;
}

size_t LineParser::parser_binary_delimiter_words(line_word_type &line_words,
                                                 const std::string &line,
                                                 size_t start, size_t end,
                                                 const delimiter_pair_type
                                                     &binary_delimiter) const
{
    size_t pos = start;
    start = start + binary_delimiter.first.size();
    if ((pos = line.find(binary_delimiter.second, start)) >= end)
        return 0;
    line_words.emplace_back(line.substr(start, pos - start),
                            BINARY_DELIMITER, binary_delimiter);
    return (pos + binary_delimiter.second.size());
}

bool LineParser::line_parser(const std::string &line,
                             line_word_type &line_words) const
{
    size_t start_pos = 0, deli_pos = 0;
    for (size_t start = 0; (start = find_unary_delimiter(line, start, line.size(),
                                                         false)) != line.size();
         start_pos = start)
    {
        for (auto iter = binary_delimiter_v.begin();
             iter != binary_delimiter_v.end(); ++iter)
        {
            if ((deli_pos = line.find(iter->first, start)) != std::string::npos)
            {
                if ((start = parser_binary_delimiter_words(
                         line_words, line, deli_pos, line.size(), *iter)) == 0)
                    return false;
                break;
            }
        }
        if (deli_pos == std::string::npos)
        {
            deli_pos = line.size();
            parser_unary_delimiter_words(line_words, line, start_pos, deli_pos);
            return true;
        }
        parser_unary_delimiter_words(line_words, line, start_pos, deli_pos);
    }
    return true;
}

} // namespace vshell