#ifndef __VSHELL_OUT_FORMAT_
#define __VSHELL_OUT_FORMAT_

#include <string>

#include "../shell_parser/shell_parser.h"
namespace vshell
{

enum FORMAT_MASK
{
    NO_FORMAT_MASK = 0x00000000,
    DATE_MASK = 0x00000010,
    TIME_MAKE = 0x00000020,
    FILENAME_MASK = 0x00000040,
    LINE_MASK = 0x00000080,
    VAR_VALUE_SHOW_MASK = 0x00000100,
    VAR_VALUE_OUT_MASK = 0x00000200,
    VERSION_MAKE = 0x00000400,
    FORCE_MAKE = 0x00000800,
    RUN_MAKE = 0x00001000,
};

class Out_format
{
  public:
    Out_format(FORMAT_MASK format_mask, const std::string &filename = "");

    bool format_out(const std::string &out_file_name, const vshell::Shell_parser::outfile_type &outf);

    static bool is_force_option(const FORMAT_MASK format);

    static bool is_run_option(const FORMAT_MASK format);

  private:
    std::string format_cmd(const size_t index, OUT_MODE out_mode, const std::string &cmd);

    bool __is_printf_date() const;

    bool __is_printf_time() const;

    bool __is_printf_filename() const;

    bool __is_printf_line() const;

    static bool __format_mask(FORMAT_MASK format, FORMAT_MASK mask);

    bool __format_mask(FORMAT_MASK mask) const;

    void __set_printf_base();

    void __set_config();

    std::string __format_show_var(const size_t index, OUT_MODE out_mode, const std::string &cmd);

    std::string __show_var_imple(const size_t pre_len, const std::string &cmd,
                                 char delimiter, const std::string newdel) const;

    std::string __get_filename_form_path(const std::string &path);

  private:
    std::string date_delimiter;
    std::string date_color_prefix;

    std::string time_delimiter;
    std::string time_color_prefix;

    std::string filename_delimiter;
    std::string filename_color_prefix;

    std::string line_delimiter;
    std::string line_color_prefix;
    size_t line_max_len;

    std::string cmd_delimter;
    std::string cmd_color_prefix;

    std::string var_deliter;
    std::string var_color_prefix;
    size_t single_var_max_len;
    size_t total_var_max_num;

  private:
    FORMAT_MASK format_mask;
    std::string filename;
    std::string color_postfix;
    std::string printf_base_prefix;
    size_t printf_base_prefix_len;
    std::string printf_base_postfix;
};
} // namespace vshell

#endif