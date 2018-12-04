#include <sys/stat.h>

#include "out_format.h"
#include "../../lib/command.h"
namespace vshell
{
using namespace std;

Out_format::Out_format(FORMAT_MASK format_mask, const string &path)
    : format_mask(format_mask),
      filename(__get_filename_form_path(path)),
      printf_base_prefix("printf \""),
      printf_base_prefix_len(0),
      printf_base_postfix(""),
      color_postfix("\\033[0m")
{
    __set_config();
    __set_printf_base();
}

bool Out_format::format_out(const string &out_file_name,
                            const vshell::Shell_parser::outfile_type &outf)
{
    ofstream ouf(out_file_name);
    if (!ouf)
        return vshell_error::open_error(out_file_name);
    for (const auto &of : outf)
    {
        if (!of.line.empty())
            ouf << format_cmd(of.index + 1, of.mode, of.line) << endl;
    }
    ouf.close();
    if (chmod(out_file_name.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IXUSR | S_IXGRP | S_IROTH | S_IXOTH) == -1)
        return vshell_error::error("chmod " + out_file_name + "executeable mode fail");
    return true;
}

string Out_format::format_cmd(const size_t index,
                              OUT_MODE out_mode,
                              const string &cmd)
{
    return __format_show_var(index, out_mode, cmd);
}

string Out_format::__get_filename_form_path(const string &path)
{
    size_t start_pos = path.find_last_of('/');
    start_pos = start_pos == string::npos ? 0 : start_pos + 1;
    return path.substr(start_pos);
}

string Out_format::__format_show_var(const size_t index,
                                     OUT_MODE out_mode,
                                     const string &cmd)
{
    if (out_mode == NORMAL)
        return (cmd);
    string printf_prefix(this->printf_base_prefix);
    string printf_postfix(this->printf_base_postfix);

    if (__is_printf_line() == true)
    {
        printf_prefix.append(this->line_delimiter)
            .append(this->line_color_prefix)
            .append("%-")
            .append(to_string(this->line_max_len))
            .append("s")
            .append(this->color_postfix);

        printf_postfix.append(" \"")
            .append(to_string(index))
            .append("\"");
    }

    size_t printf_prefix_len = this->printf_base_prefix_len +
                               this->line_delimiter.size() +
                               this->line_max_len + 1;

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
    space_str.append(string(pre_len, ' '));
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

bool Out_format::is_force_option(const FORMAT_MASK format)
{
    return __format_mask(format, FORCE_MAKE);
}

bool Out_format::is_run_option(const FORMAT_MASK format)
{
    return __format_mask(format, RUN_MAKE);
}

bool Out_format::__format_mask(FORMAT_MASK format, FORMAT_MASK mask)
{
    return (format & mask) != NO_FORMAT_MASK;
}

bool Out_format::__format_mask(FORMAT_MASK mask) const
{
    return __format_mask(this->format_mask, mask);
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