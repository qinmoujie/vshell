#ifndef _VSHELL_COMMOND_
#define _VSHELL_COMMOND_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <getopt.h>

#include "../src/format/out_format.h"
namespace vshell
{
namespace vshell_error
{
bool error(const std::string &error_messages);

bool file_line_error(const std::string &filename,
                     const size_t line_index,
                     const std::string &error_messages);

bool line_error(const size_t line_index, const std::string &error_messages);

bool open_error(const std::string &filename);
} // namespace vshell_error

bool trans_file_to_vector(const std::string &filename,
                          std::vector<std::string> &v_file);

int parse_cmdline_options(int argc, char **args, std::string &input_file,
                           std::string &output_file, FORMAT_MASK &format);

} // namespace vshell

#endif