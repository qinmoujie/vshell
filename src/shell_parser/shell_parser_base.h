#ifndef _VSHELL_SHELL_PARSER_BASE_
#define _VSHELL_SHELL_PARSER_BASE_

#include <fstream>
#include <unordered_map>

#include "../line_parser/line_parser.h"
#include "../../lib/memory_file.h"

namespace vshell
{

enum TYPE_MASK
{
    ZERO_MASK = 0x00000000,

    COMMENT_MASK = 0x00000010,
    IGNORE_START_MASK = 0x00000020,
    IGNORE_END_MASK = 0x00000040,
    START_MASK = 0x00000080,

    END_MASK = 0x00000100,
    DOUBLE_CONNECT_MASK = 0x00000200,
    ONE_CONNECT_MASK = 0x00000400,
    NOVSHELL_MASK = 0x00000800,

    FUNCTION_MASK = 0x00001000,
    FUNCTION_START_MASK = 0x00002000,
    FUNCTION_END_MASK = 0x00004000,
    IF_MASK = 0x00008000,

    THEN_MASK = 0x00010000,
    ELIF_MASK = 0x00020000,
    ELSE_MAKE = 0x00040000,
    FI_MASK = 0x00080000,

    LOOP_MASK = 0x00100000,
    DO_MASK = 0x00200000,
    DONE_MASK = 0x00400000,
    CASE_MASK = 0x00800000,
    
    IN_MASK = 0x01000000,
    CASE_OPTION_MASK = 0x02000000,
    CASE_SEMIC_MASK = 0x04000000,
    ESAC_MASK = 0x08000000,
};

class Shell_parser_base : public LineParser
{
  public:
    typedef std::vector<std::string> v_file_type;
    typedef std::pair<std::vector<std::string>, size_t> pv_file_type;
    typedef std::pair<std::string, DELIMITER_TYPE> p_word_type;
    typedef std::vector<p_word_type> v_words_type;

    // constructor
    Shell_parser_base(const v_file_type &v_file);

    // return std::string which transform from @c
    std::string to_str(const char c) const;

    // trim space begin and end of @line
    std::string trim(const std::string &line) const;

  private:
    // return true if @line is a commentary
    bool __is_commentary(const std::string &line) const;

    // @return true if line is vshell ignore start tag
    bool __is_vshell_ignore_start(const std::string &line) const;

    // @return true if line is vshell ignore start tag
    bool __is_vshell_ignore_end(const std::string &line) const;

    // @return true if line is vshell start tag
    bool __is_vshell_start_tag(const std::string &line) const;

    // @return true if line is vshell end tag
    bool __is_vshell_end_tag(const std::string &line) const;

    // @return true if line is not over by double-connector
    bool __is_double_connector(const std::string &line);

    // @return true if @line is not over by one-connetor
    bool __is_one_connector(const std::string &line) const;

    // @return true if @line is no vshell
    bool __is_novshell(const std::string &line) const;

    // @return true if @index line is function
    bool __is_func(const size_t index) const;

    // @return true if @index line is function start {
    bool __is_func_start(const size_t index) const;

    // @return true if @index line is function end }
    bool __is_func_end(const size_t index) const;

    // @return true if @index line is if
    bool __is_if(const size_t index) const;

    // @return true if @index line is then
    bool __is_then(const size_t index) const;

    // @return true if @index line is elif
    bool __is_elif(const size_t index) const;

    // @return true if @index line is elif
    bool __is_else(const size_t index) const;

    // @return true if @index line is fi
    bool __is_fi(const size_t index) const;

    // @return true if @index line is loop
    bool __is_loop(const size_t index) const;

    // @return true if @index linne is do
    bool __is_do(const size_t index) const;

    // @return true if @index line is done
    bool __is_done(const size_t index) const;

    // @return true if @index line is case
    bool __is_case(const size_t index) const;

    // @return true if @index line is in
    bool __is_in(const size_t index) const;

    // @returnn true if @index line is case )
    bool __is_case_option(const size_t index) const;

    // @return true if @index line is case ;;
    bool __is_case_semic(const size_t index) const;

    // @return true if @index line is esac
    bool __is_esac(const size_t index) const;

  private:
    // @return true if substr of line is equal target
    bool __substr_equal(const std::string &line, const std::string &target) const;

    // @return true if find @p_word in line of index
    bool __find_p_word(const size_t index, const p_word_type &p_word) const;

  private:
    // init
    void __init();

    // print word of entire file, for debug
    void __print_file_line_parser();

    // Preliminary parsing of the entire file
    bool __file_parser();

  protected:
    qmj::memory_file m_file;
    std::vector<TYPE_MASK> file_mask;

  private:
    std::unordered_set<std::string> novshell_set;

  private:
    std::vector<v_words_type> vv_file_wrod;

  private:
    std::vector<bool> is_double_connect_line;
    std::string multiLine_s;
    std::string stk;

  private:
    std::vector<p_word_type> loop_v;
};

} // namespace vshell

#endif