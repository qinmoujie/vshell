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

void __print_error_message(const std::string &error_message);

bool error(const std::string &error_messages);

int int_error(const std::string &error_message);

bool file_line_error(const std::string &filename,
                     const size_t line_index,
                     const std::string &error_messages);

bool line_error(const size_t line_index, const std::string &error_messages);

bool open_error(const std::string &filename);
} // namespace vshell_error

std::istream &trans_istream_to_vector(std::istream &inf,
                                      std::vector<std::string> &v_file);

bool trans_file_to_vector(const std::string &filename,
                          std::vector<std::string> &v_file);

int parse_cmdline_options(int argc, char **args,
                          std::vector<std::string> &input_files,
                          std::string &output_file, FORMAT_MASK &format);

std::string get_abs_workepath();

std::string trans_relative_path_to_abs(const std::string &file);

int run_shell(const std::string &cmdstrs);

} // namespace vshell

#endif