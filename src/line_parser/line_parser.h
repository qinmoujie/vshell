#ifndef _VSHLL_LINE_PARSER_
#define _VSHLL_LINE_PARSER_

#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace vshell
{

enum DELIMITER_TYPE
{
    UNARY_DELIMITER,
    BINARY_DELIMITER,
    ENABLE_PRINT_VAR,
};

struct Word_type
{
    Word_type(const std::string &value, const DELIMITER_TYPE deli_type,
              const std::pair<std::string, std::string> &delimiter)
        : value(value), deli_type(deli_type), delimiter(delimiter) {}
    typedef std::pair<size_t, size_t> point_type;

    std::string value;
    point_type point;
    DELIMITER_TYPE deli_type;
    std::pair<std::string, std::string> delimiter;
};

class LineParser
{
  public:
    typedef std::pair<std::string, std::string> delimiter_pair_type;
    typedef Word_type::point_type point_type;
    typedef std::vector<Word_type> line_word_type;

    // default constructor
    LineParser();

    bool line_parser(const std::vector<std::string> &inf,
                     std::vector<line_word_type> &ouf);

  protected:
    // set of binary delimiter
    std::vector<delimiter_pair_type> binary_delimiter_v;

    std::vector<delimiter_pair_type> variable_delimiter_v;
    // set of unary delimiter
    std::unordered_set<char> unary_delimiter_set;

    std::vector<bool> is_double_connect_line;

  private:
    // @return @word is or not a unary delimiter word
    bool is_unary_delimiter(const char word) const;

    // find first position which is or not unary delimiter
    // in [@start,@end) of @line by @is_unary
    // @return @end if not found, else return the position in @line
    size_t find_unary_delimiter(const std::string &line,
                                size_t start, size_t end,
                                bool is_unary) const;

    bool is_binary_delimiter_start(const std::vector<std::string> &inf,
                                   const size_t line_index, const size_t column_index,
                                   delimiter_pair_type &delimiter_pair);


    // insert unary and binary delimiter word
    void __init_delimiter_set();
};

} // namespace vshell
#endif