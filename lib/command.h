#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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

} // namespace vshell