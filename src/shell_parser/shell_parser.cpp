#include "shell_parser.h"
#include "../../lib/command.h"

namespace vshell
{

Shell_parser::Shell_parser(const v_file_type &v_file, const string &input_file_name)
    : base_type(v_file), input_file_name(input_file_name) {}

// return true if @line is a commentary
bool Shell_parser::is_commentary(const size_t index) const
{
    return __mask(index, COMMENT_MASK);
}

// @return true if line is vshell ignore start tag
bool Shell_parser::is_vshell_ignore_start(const size_t index) const
{
    return __mask(index, IGNORE_START_MASK);
}

// @return true if line is vshell ignore start tag
bool Shell_parser::is_vshell_ignore_end(const size_t index) const
{
    return __mask(index, IGNORE_END_MASK);
}

// @return true if line is vshell start tag
bool Shell_parser::is_vshell_start_tag(const size_t index) const
{
    return __mask(index, START_MASK);
}

// @return true if line is vshell end tag
bool Shell_parser::is_vshell_end_tag(const size_t index) const
{
    return __mask(index, END_MASK);
}

// @return true if line is not over by double-connector
bool Shell_parser::is_double_connector(const size_t index) const
{
    return __mask(index, DOUBLE_CONNECT_MASK);
}

// @return true if @line is not over by one-connetor
bool Shell_parser::is_one_connector(const size_t index) const
{
    return __mask(index, ONE_CONNECT_MASK);
}

// @return true if @line is no vshell
bool Shell_parser::is_novshell(const size_t index) const
{
    return __mask(index, NOVSHELL_MASK);
}

// @return true if @index line is function
bool Shell_parser::is_func(const size_t index) const
{
    return __mask(index, FUNCTION_MASK);
}

// @return true if @index line is function start {
bool Shell_parser::is_func_start(const size_t index) const
{
    return __mask(index, FUNCTION_START_MASK);
}

// @return true if @index line is function end }
bool Shell_parser::is_func_end(const size_t index) const
{
    return __mask(index, FUNCTION_END_MASK);
}

// @return true if @index line is if
bool Shell_parser::is_if(const size_t index) const
{
    return __mask(index, IF_MASK);
}

// @return true if @index line is then
bool Shell_parser::is_then(const size_t index) const
{
    return __mask(index, THEN_MASK);
}

// @return true if @index line is elif
bool Shell_parser::is_elif(const size_t index) const
{
    return __mask(index, ELIF_MASK);
}

// @return true if @index line is else
bool Shell_parser::is_else(const size_t index) const
{
    return __mask(index, ELSE_MAKE);
}

// @return true if @index line is fi
bool Shell_parser::is_fi(const size_t index) const
{
    return __mask(index, FI_MASK);
}

// @return true if @index line is loop
bool Shell_parser::is_loop(const size_t index) const
{
    return __mask(index, LOOP_MASK);
}

// @return true if @index linne is do
bool Shell_parser::is_do(const size_t index) const
{
    return __mask(index, DO_MASK);
}

// @return true if @index line is done
bool Shell_parser::is_done(const size_t index) const
{
    return __mask(index, DONE_MASK);
}

// @return true if @index line is case
bool Shell_parser::is_case(const size_t index) const
{
    return __mask(index, CASE_MASK);
}

// @return true if @index line is in
bool Shell_parser::is_in(const size_t index) const
{
    return __mask(index, IN_MASK);
}

// @returnn true if @index line is case )
bool Shell_parser::is_case_option(const size_t index) const
{
    return __mask(index, CASE_OPTION_MASK);
}

// @return true if @index line is case ;;
bool Shell_parser::is_case_semic(const size_t index) const
{
    return __mask(index, CASE_SEMIC_MASK);
}

// @return true if @index line is esac
bool Shell_parser::is_esac(const size_t index) const
{
    return __mask(index, ESAC_MASK);
}

bool Shell_parser::parser_vshell_ignore(qmj::memory_file &inf,
                                        outfile_type &ouf,
                                        const size_t start_index,
                                        string &buf)
{
    size_t abs_index = start_index;
    do
    {
        ouf.emplace_back(abs_index, NORMAL, buf);
        if (inf.getline(buf) == false)
            return __file_line_error(start_index,
                                     "not found '" +
                                         this->vshell_ignore_end_tag + "'");
        abs_index += 1;
    } while (is_vshell_ignore_end(abs_index) == false);
    ouf.emplace_back(abs_index, NORMAL, buf);
    return true;
}

bool Shell_parser::parser_double_connector(qmj::memory_file &inf,
                                           outfile_type &ouf,
                                           const size_t start_index,
                                           string &buf)
{
    size_t abs_index(inf.get_absindex() - 1);
    string newline = buf;
    do
    {
        if (inf.getline(buf) == false)
            return __file_line_error(start_index, "double connector not close");
        abs_index += 1;
        newline.append("\n").append(buf);
    } while (is_double_connector(abs_index) == true);
    if (is_one_connector(abs_index))
        return parser_one_connector(inf, ouf, start_index, newline);
    else
        ouf.emplace_back(start_index, V_FIRST, newline);
    return true;
}

bool Shell_parser::parser_one_connector(qmj::memory_file &inf,
                                        outfile_type &ouf,
                                        const size_t start_index,
                                        string &buf)
{
    size_t abs_index(inf.get_absindex() - 1);
    string newline = buf;
    do
    {
        if (inf.getline(buf) == false)
            return __file_line_error(abs_index, "too many one conector");
        abs_index += 1;
        newline.append("\n").append(buf);
    } while (is_one_connector(abs_index) == true);
    if (is_double_connector(abs_index))
        return parser_double_connector(inf, ouf, start_index, newline);
    else
        ouf.emplace_back(start_index, V_FIRST, newline);
    return true;
}

bool Shell_parser::parser_if(qmj::memory_file &inf,
                             outfile_type &ouf,
                             const size_t start_index,
                             string &buf)
{
    string newline = buf;
    size_t abs_index(start_index);
    for (; is_then(abs_index) == false; ++abs_index)
    {
        if (inf.getline(buf) == false)
            return __file_line_error(start_index, "not found 'then'");
        newline.append("\n").append(buf);
    }
    ouf.emplace_back(start_index, V_FIRST, newline);
    if (is_fi(abs_index) == true)
        return true;

    size_t stk = 1;
    for (size_t if_start = (++abs_index); inf.getline(buf); abs_index = inf.get_absindex())
    {
        if (is_if(abs_index) == true)
        {
            stk += 1;
        }
        if (is_elif(abs_index) == true)
        {
            if (stk == 1)
            {
                qmj::memory_file elif_inf(inf, if_start, abs_index);
                if (__parser_imple(elif_inf, ouf) == false)
                    return false;

                size_t elif_start = abs_index;
                newline = buf;
                for (; is_then(abs_index) == false; ++abs_index)
                {
                    if (inf.getline(buf) == false)
                        return __file_line_error(elif_start, "not found 'then'");
                    newline.append("\n").append(buf);
                }
                ouf.emplace_back(elif_start, V_FIRST, newline);
                if_start = abs_index + 1;
            }
        }
        if (is_else(abs_index) == true)
        {
            if (stk == 1)
            {
                qmj::memory_file else_inf(inf, if_start, abs_index);
                if (__parser_imple(else_inf, ouf) == false)
                    return false;
                ouf.emplace_back(abs_index, V_SECOND, buf);
                if_start = abs_index + 1;
            }
        }
        if (is_fi(abs_index) == true)
        {
            if (--stk == 0)
            {
                qmj::memory_file fi_inf(inf, if_start, abs_index);
                if (__parser_imple(fi_inf, ouf) == false)
                    return false;
                ouf.emplace_back(abs_index, V_FIRST, buf);
                return true;
            }
        }
        else
        {
        }
    }
    return __file_line_error(start_index, "not found 'fi'");
}

bool Shell_parser::parser_loop(qmj::memory_file &inf,
                               outfile_type &ouf,
                               const size_t start_index,
                               string &buf)
{
    string newline = buf;
    size_t abs_index(start_index);
    for (; is_do(abs_index) == false; abs_index += 1)
    {
        if (inf.getline(buf) == false)
            return __file_line_error(start_index, "not found 'do'");
        newline.append("\n").append(buf);
    }
    ouf.emplace_back(start_index, V_FIRST, newline);

    size_t stk = 1;
    for (size_t loop_start = (++abs_index); inf.getline(buf); abs_index = inf.get_absindex())
    {
        if (is_loop(abs_index) == true)
        {
            stk += 1;
        }
        else if (is_done(abs_index) == true)
        {
            if (--stk == 0)
            {
                qmj::memory_file loop_inf(inf, loop_start, abs_index);
                if (__parser_imple(loop_inf, ouf) == false)
                    return false;
                ouf.emplace_back(abs_index, V_SECOND, buf);
                return true;
            }
        }
        else
        {
        }
    }
    return __file_line_error(start_index, "not found 'done'");
}

bool Shell_parser::parser_func(qmj::memory_file &inf,
                               outfile_type &ouf,
                               const size_t start_index,
                               string &buf)
{
    size_t abs_index(start_index);
    string newline = buf;
    if (trim(buf).back() != '{')
    {
        do
        {
            if (inf.getline(buf) == false)
                return __file_line_error(start_index, "not found '{'");
            abs_index += 1;
            newline.append("\n").append(buf);
        } while (is_func_start(abs_index) == false);
    }
    ouf.emplace_back(start_index, V_SECOND, newline);

    for (size_t func_start_index = ++abs_index; inf.getline(buf); abs_index = inf.get_absindex())
    {
        if (is_func_end(abs_index) == true)
        {
            qmj::memory_file func_file(inf, func_start_index, abs_index);
            if (__parser_imple(func_file, ouf) == false)
                return false;
            ouf.emplace_back(abs_index, V_FIRST, buf);
            return true;
        }
    }
    return __file_line_error(start_index, "not found '}'");
}

bool Shell_parser::parser_case(qmj::memory_file &inf,
                               outfile_type &ouf,
                               const size_t start_index,
                               string &buf)
{
    string newline = buf;
    size_t abs_index(start_index);
    for (; is_in(abs_index) == false; abs_index = inf.get_absindex())
    {
        if (inf.getline(buf) == false)
            return __file_line_error(start_index, "not found 'in'");
        newline.append("\n").append(buf);
    }
    ouf.emplace_back(start_index, V_FIRST, newline);

    size_t in_start = abs_index + 1;
    size_t stk = 1;
    for (size_t case_start = ++abs_index; inf.getline(buf); abs_index = inf.get_absindex())
    {
        if (is_case(abs_index))
        {
            stk += 1;
        }
        else if (is_case_option(abs_index) == true)
        {
            if (stk == 1)
            {
                if (case_start != in_start)
                {
                    qmj::memory_file case_option_inf(inf, case_start, abs_index);
                    if (__parser_imple(case_option_inf, ouf) == false)
                        return false;
                }
                ouf.emplace_back(abs_index, V_SECOND, buf);
                case_start = abs_index + 1;
            }
        }
        else if (is_esac(abs_index) == true)
        {
            if (--stk == 0)
            {
                qmj::memory_file esac_inf(inf, case_start, abs_index);
                if (__parser_imple(esac_inf, ouf) == false)
                    return false;
                ouf.back().line.append("\n").append(buf);
                //& ouf.emplace_back(abs_index, V_FIRST, buf);
                return true;
            }
        }
        else
        {
        }
    }
    return __file_line_error(start_index, "not found 'esac'");
}

bool Shell_parser::parser_vshell(qmj::memory_file &inf,
                                 outfile_type &ouf,
                                 const size_t start_index,
                                 string &buf)
{
    size_t abs_index = start_index;
    string vshell_line;
    ouf.emplace_back(abs_index, NORMAL, buf);
    for (;;)
    {
        if (inf.getline(buf) == false)
            return __file_line_error(start_index, "not found '" + this->vshell_end_tag + "'");
        abs_index += 1;
        if (is_vshell_end_tag(abs_index) == true)
            break;
        vshell_line.append(buf).append("\n");
    }
    if (!vshell_line.empty())
    {
        vshell_line.pop_back();
        ouf.emplace_back(abs_index, V_FIRST, vshell_line);
    }
    ouf.emplace_back(abs_index, NORMAL, buf);
    return true;
}

bool Shell_parser::__parser_imple(qmj::memory_file &inf, outfile_type &ouf)
{
    inf.reopend();
    string buf;
    for (size_t cur_abs_index = inf.get_absindex(); inf.getline(buf); cur_abs_index = inf.get_absindex())
    {
        if (is_commentary(cur_abs_index) == true)
        {
            if (is_vshell_ignore_start(cur_abs_index) == true)
            {
                if (parser_vshell_ignore(inf, ouf, cur_abs_index, buf) == false)
                    return false;
            }
            else if (is_vshell_start_tag(cur_abs_index) == true)
            {
                if (parser_vshell(inf, ouf, cur_abs_index, buf) == false)
                    return false;
            }
            else
            {
                ouf.emplace_back(cur_abs_index, NORMAL, buf);
            }
        }
        else
        {
            if (is_if(cur_abs_index))
            {
                if (parser_if(inf, ouf, cur_abs_index, buf) == false)
                    return false;
            }
            else if (is_func(cur_abs_index) == true)
            { // has function
                if (parser_func(inf, ouf, cur_abs_index, buf) == false)
                    return false;
            }
            else if (is_case(cur_abs_index) == true)
            {
                if (parser_case(inf, ouf, cur_abs_index, buf) == false)
                    return false;
            }
            else if (is_loop(cur_abs_index) == true)
            {
                if (parser_loop(inf, ouf, cur_abs_index, buf) == false)
                    return false;
            }
            else if (is_double_connector(cur_abs_index) == true)
            { // a cmd is multi line
                if (parser_double_connector(inf, ouf, cur_abs_index, buf) == false)
                    return false;
            }
            else if (is_one_connector(cur_abs_index) == true)
            {
                if (parser_one_connector(inf, ouf, cur_abs_index, buf) == false)
                    return false;
            }
            else
            {
                ouf.emplace_back(cur_abs_index, V_FIRST, buf);
            }
        }
    }
    return true;
}

bool Shell_parser::parser(outfile_type &ouf)
{
    return __parser_imple(m_file, ouf);
}

bool Shell_parser::__mask(const size_t index, const TYPE_MASK t_mask) const
{
    return (this->file_mask[index] & t_mask) != ZERO_MASK;
}

bool Shell_parser::__file_line_error(const size_t index, const string &messages) const
{
    return vshell_error::file_line_error(input_file_name, index + 1, messages);
}

} // namespace vshell
