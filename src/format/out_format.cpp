#include "out_format.h"

namespace vshell
{
using namespace std;

Out_format::Out_format(FORMAT_MASK format_mask, const string &filename)
    : format_mask(format_mask),
      filename(filename),
      printf_base_prefix("printf \""),
      printf_base_prefix_len(0),
      printf_base_postfix(""),
      color_postfix("\\033[0m")
{
    __set_config();
    __set_printf_base();
}

string Out_format::format_cmd(const size_t index,
                              OUT_MODE out_mode,
                              const string &cmd)
{
    return __format_show_var(index, out_mode, cmd);
}

string Out_format::__format_show_var(const size_t index,
                                     OUT_MODE out_mode,
                                     const string &cmd)
{
    if (out_mode == NORMAL)
        return (cmd);
    string printf_prefix(this->printf_base_prefix);
    printf_prefix.append(this->line_delimiter)
        .append(this->line_color_prefix)
        .append("%-")
        .append(to_string(this->line_max_len))
        .append("s")
        .append(this->color_postfix);
    size_t printf_prefix_len = this->printf_base_prefix_len +
                               this->line_delimiter.size() + this->line_max_len;
    string printf_postfix(this->printf_base_postfix);
    printf_postfix.append(" \"")
        .append(to_string(index))
        .append("\"");

    printf_prefix.append(this->cmd_delimter)
        .append(this->cmd_color_prefix)
        .append("%s")
        .append(this->color_postfix)
        .append("\n\"");

    string cmd_show_var = __show_var_imple(printf_prefix_len, cmd, '"', "\\\"");
    printf_postfix.append(" ").append(cmd_show_var);

    if (out_mode == V_FIRST)
    {
        return (printf_prefix.append(" ")
                    .append(printf_postfix)
                    .append("\n")
                    .append(cmd));
    }
    else if (out_mode == V_SECOND)
    {
        return (string(cmd).append("\n").append(printf_prefix).append(" ").append(printf_postfix));
    }
    return (cmd);
}

string Out_format::__show_var_imple(const size_t pre_len,
                                    const string &cmd,
                                    char delimiter,
                                    const string newdel) const
{
    string result;
    result.reserve(cmd.size());
    string space_str("\n");
    space_str.append(string(this->printf_base_prefix_len, ' '));
    if (!cmd.empty())
    {
        if (cmd[0] == delimiter)
            result = newdel;
        else
        {
            result.push_back(cmd[0]);
        }
        for (size_t i = 1; i != cmd.size(); ++i)
        {
            if (cmd[i] == '\n')
            {
                result.append(space_str);
            }
            else if (cmd[i] == delimiter && cmd[i - 1] != '\\')
            {
                result.append(newdel);
            }
            else
            {
                result.push_back(cmd[i]);
            }
        }
    }
    return (delimiter + result + delimiter);
}

bool Out_format::__is_printf_date() const
{
    return __format_mask(DATE_MASK);
}

bool Out_format::__is_printf_time() const
{
    return __format_mask(TIME_MAKE);
}

bool Out_format::__is_printf_filename() const
{
    return __format_mask(FILENAME_MASK);
}

bool Out_format::__is_printf_line() const
{
    return __format_mask(LINE_MASK);
}

bool Out_format::__format_mask(FORMAT_MASK mask) const
{
    return (this->format_mask & mask) != NO_FORMAT_MASK;
}

void Out_format::__set_config()
{
    this->date_delimiter = "|";
    this->date_color_prefix = "\\033[2;3;36m";

    this->time_delimiter = " ";
    this->time_color_prefix = "\\033[2;3;36m";

    this->filename_delimiter = "|";
    this->filename_color_prefix = "\\033[2;3;34m";

    this->line_delimiter = ":";
    this->line_color_prefix = "\\033[2;3;34m";
    this->line_max_len = 6;

    this->cmd_delimter = "|";
    this->cmd_color_prefix = "\\033[1;2;3m";

    this->var_deliter = "->";
    this->var_color_prefix = "\\033[2;3;34m";
    this->single_var_max_len = 10;
    this->total_var_max_num = 3;
}

void Out_format::__set_printf_base()
{
    if (__is_printf_date() == true)
    {
        this->printf_base_prefix.append(this->date_delimiter)
            .append(this->date_color_prefix)
            .append("%s")
            .append(this->color_postfix);

        this->printf_base_postfix.append(" \"$(date \"+%Y-%m-%d\")\"");
        this->printf_base_prefix_len += (this->date_delimiter.size() + 10);
    }

    if (__is_printf_time() == true)
    {
        this->printf_base_prefix.append(this->time_delimiter)
            .append(this->time_color_prefix)
            .append("%s")
            .append(this->color_postfix);

        this->printf_base_postfix.append(" \"$(date \"+%H:%M:%S\")\"");
        this->printf_base_prefix_len += (this->time_delimiter.size() + 8);
    }

    if (__is_printf_filename() == true)
    {
        this->printf_base_prefix.append(this->filename_delimiter)
            .append(this->filename_color_prefix)
            .append("%s")
            .append(this->color_postfix);

        this->printf_base_postfix.append(" \"")
            .append(this->filename)
            .append("\"");
        this->printf_base_prefix_len += (this->filename_delimiter.size() + this->filename.size());
    }
}

} // namespace vshell