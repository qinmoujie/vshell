#include "command.h"

using namespace std;

namespace vshell
{
bool vshell_error::error(const string &error_messages)
{
    std::cerr << "vshell: " + error_messages << std::endl;
    return false;
}

bool vshell_error::line_error(const size_t line_index, const string &error_messages)
{
    return error("line " + to_string(line_index) + " " + error_messages);
}

bool vshell_error::file_line_error(const string &filename,
                                   const size_t line_index,
                                   const string &error_messages)
{
    return error(filename + ":" + to_string(line_index) + ": " + error_messages);
}

bool vshell_error::open_error(const string &filename)
{
    return error("open error: " + filename);
}

bool trans_file_to_vector(const string &filename, vector<string> &v_file)
{
    ifstream inf(filename);
    if (!inf)
        return vshell_error::open_error(filename);
    vector<string> temp_file;
    string buf;
    for (; getline(inf, buf);)
        temp_file.emplace_back(std::move(buf));
    v_file.swap(temp_file);
    inf.close();
    return true;
}

} // namespace vshell