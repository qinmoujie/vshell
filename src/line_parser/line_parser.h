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
};

struct Word_type
{
    Word_type(const std::string &value, const DELIMITER_TYPE deli_type,
              const std::pair<std::string, std::string> &delimiter)
        : value(value), deli_type(deli_type), delimiter(delimiter) {}

    std::string value;
    DELIMITER_TYPE deli_type;
    std::pair<std::string, std::string> delimiter;
};

class LineParser
{
  public:
    typedef std::pair<std::string, std::string> delimiter_pair_type;
    typedef std::vector<Word_type> line_word_type;

    // default constructor
    LineParser();

    // @return @word is or not a unary delimiter word
    bool is_unary_delimiter(const char word) const;

    // find first position which is or not unary delimiter
    // in [@start,@end) of @line by @is_unary
    // @return @end if not found, else return the position in @line
    size_t find_unary_delimiter(const std::string &line,
                                size_t start, size_t end,
                                bool is_unary) const;

    // parser [@start,@end) of @line delimit by unary delimiter
    // and insert @line_words
    // @return ture if success, else return false
    bool parser_unary_delimiter_words(line_word_type &line_words,
                                      const std::string &line,
                                      size_t start, size_t end) const;

    // parser [@start,@end) of @line delimit
    // by binary delimiter @binary_delimiter
    // and insert @line_words
    // @return pos if success, else return 0
    size_t parser_binary_delimiter_words(line_word_type &line_words,
                                         const std::string &line,
                                         size_t start, size_t end,
                                         const delimiter_pair_type
                                             &binary_delimiter) const;
    // parser @line and insert @line_words
    // @return ture if success, else return false
    bool line_parser(const std::string &line, line_word_type &line_words) const;

  protected:
    // set of binary delimiter
    std::vector<delimiter_pair_type> binary_delimiter_v;
    // set of unary delimiter
    std::unordered_set<char> unary_delimiter_set;

  private:
    // insert unary and binary delimiter word
    void __init_delimiter_set();
};

} // namespace vshell
#endif