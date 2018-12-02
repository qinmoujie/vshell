#ifndef _VSHELL_SHELL_PARSER_
#define _VSHELL_SHELL_PARSER_

#include <fstream>
#include <unordered_map>

#include "shell_parser_base.h"

namespace vshell
{

using namespace std;

enum OUT_MODE
{
    NORMAL,
    V_FIRST,
    V_SECOND,
};

struct Out_info
{
    Out_info(const size_t index, const OUT_MODE mode, const string &line)
        : index(index), mode(mode), line(line) {}

    size_t index;
    OUT_MODE mode;
    string line;
};

class Shell_parser : public Shell_parser_base
{
  public:
    typedef Shell_parser_base base_type;
    typedef base_type::v_file_type v_file_type;
    typedef base_type::pv_file_type pv_file_type;
    typedef base_type::p_word_type p_word_type;
    typedef base_type::v_words_type v_words_type;
    typedef vector<Out_info> outfile_type;

    Shell_parser(const v_file_type &v_file, const std::string &input_file_name);

    // return true if @line is a commentary
    bool is_commentary(const size_t index) const;

    // @return true if line is vshell ignore start tag
    bool is_vshell_ignore_start(const size_t index) const;

    // @return true if line is vshell ignore start tag
    bool is_vshell_ignore_end(const size_t index) const;

    // @return true if line is vshell start tag
    bool is_vshell_start_tag(const size_t index) const;

    // @return true if line is vshell end tag
    bool is_vshell_end_tag(const size_t index) const;

    // @return true if line is not over by double-connector
    bool is_double_connector(const size_t index) const;

    // @return true if @line is not over by one-connetor
    bool is_one_connector(const size_t index) const;

    // @return true if @line is no vshell
    bool is_novshell(const size_t index) const;

    // @return true if @index line is function
    bool is_func(const size_t index) const;

    // @return true if @index line is function start {
    bool is_func_start(const size_t index) const;

    // @return true if @index line is function end }
    bool is_func_end(const size_t index) const;

    // @return true if @index line is if
    bool is_if(const size_t index) const;

    // @return true if @index line is then
    bool is_then(const size_t index) const;

    // @return true if @index line is elif
    bool is_elif(const size_t index) const;

    // @return true if @index line is else
    bool is_else(const size_t index) const;

    // @return true if @index line is fi
    bool is_fi(const size_t index) const;

    // @return true if @index line is loop
    bool is_loop(const size_t index) const;

    // @return true if @index linne is do
    bool is_do(const size_t index) const;

    // @return true if @index line is done
    bool is_done(const size_t index) const;

    // @return true if @index line is case
    bool is_case(const size_t index) const;

    // @return true if @index line is in
    bool is_in(const size_t index) const;

    // @returnn true if @index line is case )
    bool is_case_option(const size_t index) const;

    // @return true if @index line is case ;;
    bool is_case_semic(const size_t index) const;

    // @return true if @index line is esac
    bool is_esac(const size_t index) const;

  public:
    bool parser_vshell_ignore(qmj::memory_file &inf,
                              outfile_type &ouf,
                              const size_t start_index,
                              string &buf);

    bool parser_vshell(qmj::memory_file &inf,
                       outfile_type &ouf,
                       const size_t start_index,
                       string &buf);

    bool parser_double_connector(qmj::memory_file &inf,
                                 outfile_type &ouf,
                                 const size_t start_index,
                                 string &buf);

    bool parser_one_connector(qmj::memory_file &inf,
                              outfile_type &ouf,
                              const size_t start_index,
                              string &buf);

    bool parser_if(qmj::memory_file &inf,
                   outfile_type &ouf,
                   const size_t start_index,
                   string &buf);

    bool parser_loop(qmj::memory_file &inf,
                     outfile_type &ouf,
                     const size_t start_index,
                     string &buf);

    bool parser_func(qmj::memory_file &inf,
                     outfile_type &ouf,
                     const size_t start_index,
                     string &buf);

    bool parser_case(qmj::memory_file &inf,
                     outfile_type &ouf,
                     const size_t start_index,
                     string &buf);

    bool parser(outfile_type &ouf);

  private:
    // @return true if @index line has @t_mask
    bool __mask(const size_t index, const TYPE_MASK t_mask) const;

    bool __file_line_error(const size_t index, const string &messages) const;

    bool __parser_imple(qmj::memory_file &inf, outfile_type &ouf);

  private:
    const std::string input_file_name;
};

} // namespace vshell

#endif