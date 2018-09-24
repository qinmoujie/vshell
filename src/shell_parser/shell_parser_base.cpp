#include "shell_parser_base.h"

namespace vshell
{

using namespace std;

Shell_parser_base::Shell_parser_base(const v_file_type &v_file)
    : LineParser::LineParser(),
      m_file(v_file),
      file_mask(v_file.size(), TYPE_MASK(0)),
      is_double_connect_line(v_file.size(), false),
      multiLine_s("\"'`"),
      stk("")
{
    __init();
}

void Shell_parser_base::__init()
{
    novshell_set.insert("");
    novshell_set.insert("esac");
    loop_v.push_back({"while", UNARY_DELIMITER});
    loop_v.push_back({"for", UNARY_DELIMITER});
    loop_v.push_back({"until", UNARY_DELIMITER});
    __file_parser();
    //__print_file_line_parser();
}

string Shell_parser_base::to_str(const char c) const
{
    char temp[2];
    temp[1] = '\0';
    temp[0] = c;
    return string(temp);
}

void Shell_parser_base::__print_file_line_parser()
{
    this->m_file.reopend();
    size_t index = 0;
    for (const auto &del_v : vv_file_wrod)
    {
        cout << "==================" << endl;
        cout << "line:" << to_string(index + 1) << " " << m_file.getline(index) << endl;
        ++index;
        for (auto simp_word : del_v)
        {
            cout << "value: " << simp_word.first << endl;
            cout << "DELIMITER_TYPE: ";
            simp_word.second == UNARY_DELIMITER ? cout << "UNARY_DELIMITER" << endl
                                                : cout << "BINARY_DELIMITER" << endl;
        }
        cout << "===================" << endl;
    }
    this->m_file.reopend();
}

bool Shell_parser_base::__is_commentary(const string &line) const
{
    for (const char c : line)
    {
        if (!isspace(c))
        {
            return c == '#';
        }
    }
    return false;
}

bool Shell_parser_base::__is_vshell_ignore_start(const string &line) const
{
    return __substr_equal(line, "##vshell_ignore_start");
}

bool Shell_parser_base::__is_vshell_ignore_end(const string &line) const
{
    return __substr_equal(line, "##vshell_ignore_end");
}

bool Shell_parser_base::__is_vshell_start_tag(const string &line) const
{
    return __substr_equal(line, "##vshell_start_tag");
}

bool Shell_parser_base::__is_vshell_end_tag(const string &line) const
{
    return __substr_equal(line, "##vshell_end_tag");
}

bool Shell_parser_base::__is_double_connector(const string &line)
{
    if (__is_commentary(line) == false)
    {
        for (size_t pos = 0;
             (pos = line.find_first_of(multiLine_s, pos)) != std::string::npos;
             pos += 1)
        {
            if (pos == 0 || line[pos - 1] != '\\')
            {
                if (!this->stk.empty() && this->stk.back() == line[pos])
                    this->stk.pop_back();
                else
                    this->stk.append(to_str(line[pos]));
            }
        }
    }
    if (this->stk.empty() || (this->stk.size() != 1 && this->stk.front() == this->stk.back()))
    {
        this->stk = "";
        return false;
    }
    return true;
}

bool Shell_parser_base::__is_one_connector(const std::string &line) const
{
    for (auto riter = line.crbegin(); riter != line.crend(); ++riter)
        if (!isspace(*riter))
            return (*riter == '\\') || (*riter == '|');
    return (false);
}

string Shell_parser_base::trim(const std::string &line) const
{
    size_t start = 0, end = line.size();
    for (; start != end && isspace(line[start]); ++start)
        ;
    for (; start != end && isspace(line[end - 1]); --end)
        ;
    return line.substr(start, end - start);
}

bool Shell_parser_base::__substr_equal(const string &line, const string &target) const
{
    return line.substr(0, target.size()) == target;
}

bool Shell_parser_base::__is_novshell(const std::string &line) const
{
    return novshell_set.find(trim(line)) != novshell_set.end();
}

bool Shell_parser_base::__is_func(const size_t index) const
{
    return __find_p_word(index, p_word_type("function", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_func_start(const size_t index) const
{
    return this->is_double_connect_line[index] == false &&
           __find_p_word(index, p_word_type("{", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_func_end(const size_t index) const
{
    return this->is_double_connect_line[index] == false &&
           __find_p_word(index, p_word_type("}", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_if(const size_t index) const
{
    return __find_p_word(index, p_word_type("if", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_then(const size_t index) const
{
    return __find_p_word(index, p_word_type("then", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_elif(const size_t index) const
{
    return __find_p_word(index, p_word_type("elif", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_else(const size_t index) const
{
    return __find_p_word(index, p_word_type("else", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_fi(const size_t index) const
{
    return __find_p_word(index, p_word_type("fi", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_loop(const size_t index) const
{
    for (auto iter = this->loop_v.begin(); iter != this->loop_v.end(); ++iter)
    {
        if (__find_p_word(index, *iter) == true)
        {
            return true;
        }
    }
    return false;
}

bool Shell_parser_base::__is_do(const size_t index) const
{
    return __find_p_word(index, p_word_type("do", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_done(const size_t index) const
{
    return __find_p_word(index, p_word_type("done", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_case(const size_t index) const
{
    return __find_p_word(index, p_word_type("case", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_in(const size_t index) const
{
    return __find_p_word(index, p_word_type("in", UNARY_DELIMITER));
}

bool Shell_parser_base::__is_case_option(const size_t index) const
{
    return trim(m_file.get_absline(index)).back() == ')';
}

bool Shell_parser_base::__is_case_semic(const size_t index) const
{
    return trim(m_file.get_absline(index)) == ";;";
}

bool Shell_parser_base::__is_esac(const size_t index) const
{
    return trim(m_file.get_absline(index)) == "esac";
}

bool Shell_parser_base::__find_p_word(const size_t index,
                                      const p_word_type &p_word) const
{
    for (const p_word_type &p_w : vv_file_wrod[index])
    {
        if (p_w == p_word)
            return true;
    }
    return false;
}

bool Shell_parser_base::__file_parser()
{
    string buf;
    m_file.reopend();
    for (size_t index = 0; m_file.getline(buf); ++index)
    {
        TYPE_MASK &index_mask = file_mask[index];

        if (__is_double_connector(buf) == true)
        {
            index_mask = TYPE_MASK(index_mask | DOUBLE_CONNECT_MASK);
            this->is_double_connect_line[index] = true;
        }

        line_word_type words;
        this->line_parser(buf, words);
        v_words_type v_words;
        for (const Word_type &w : words)
        {
            v_words.emplace_back(w.value, w.deli_type);
        }
        vv_file_wrod.push_back(std::move(v_words));

        if (__is_novshell(buf) == true)
            index_mask = TYPE_MASK(index_mask | NOVSHELL_MASK);

        if (__is_commentary(buf) == true)
        {
            index_mask = TYPE_MASK(index_mask | COMMENT_MASK);

            if (__is_vshell_ignore_start(buf) == true)
                index_mask = TYPE_MASK(index_mask | IGNORE_START_MASK);
            if (__is_vshell_ignore_end(buf) == true)
                index_mask = TYPE_MASK(index_mask | IGNORE_END_MASK);

            if (__is_vshell_start_tag(buf) == true)
                index_mask = TYPE_MASK(index_mask | START_MASK);
            if (__is_vshell_end_tag(buf) == true)
                index_mask = TYPE_MASK(index_mask | END_MASK);
        }
        else
        {
            if (__is_one_connector(buf) == true)
                index_mask = TYPE_MASK(index_mask | ONE_CONNECT_MASK);
            if (__is_func(index) == true)
                index_mask = TYPE_MASK(index_mask | FUNCTION_MASK);
            if (__is_func_start(index) == true)
                index_mask = TYPE_MASK(index_mask | FUNCTION_START_MASK);
            if (__is_func_end(index) == true)
                index_mask = TYPE_MASK(index_mask | FUNCTION_END_MASK);

            if (__is_if(index) == true)
                index_mask = TYPE_MASK(index_mask | IF_MASK);
            if (__is_then(index) == true)
                index_mask = TYPE_MASK(index_mask | THEN_MASK);
            if (__is_elif(index) == true)
                index_mask = TYPE_MASK(index_mask | ELIF_MASK);
            if (__is_fi(index) == true)
                index_mask = TYPE_MASK(index_mask | FI_MASK);

            if (__is_loop(index) == true)
                index_mask = TYPE_MASK(index_mask | LOOP_MASK);
            if (__is_do(index) == true)
                index_mask = TYPE_MASK(index_mask | DO_MASK);
            if (__is_done(index) == true)
                index_mask = TYPE_MASK(index_mask | DONE_MASK);

            if (__is_case(index) == true)
                index_mask = TYPE_MASK(index_mask | CASE_MASK);
            if (__is_in(index) == true)
                index_mask = TYPE_MASK(index_mask | IN_MASK);
            if (__is_case_option(index) == true)
                index_mask = TYPE_MASK(index_mask | CASE_OPTION_MASK);
            if (__is_case_semic(index) == true)
                index_mask = TYPE_MASK(index_mask | CASE_SEMIC_MASK);
            if (__is_esac(index) == true)
                index_mask = TYPE_MASK(index_mask | ESAC_MASK);
        }
        //@       std::cout << index << ":" << is_double_connect_line[index] << "|" << index_mask << "|" << buf << std::endl;
    }
    m_file.reopend();
    return true;
}

} // namespace vshell
