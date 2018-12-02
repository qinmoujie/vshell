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

int parse_cmdline_options(int argc, char **args, string &input_file,
                           string &output_file, FORMAT_MASK &format)
{
    int opt;
    static struct option long_options[] =
        {
            {"version", no_argument, NULL, 'V'},
            {"date", no_argument, NULL, 'd'},
            {"filename", no_argument, NULL, 'f'},
            {"line", no_argument, NULL, 'l'},
            {"input-file", required_argument, NULL, 'i'},
            {"output-file", required_argument, NULL, 'o'},
            {"time", no_argument, NULL, 't'},
        };
    for (; (opt = getopt_long(argc, args, "Vdfli:o:t", long_options, NULL)) != -1;)
    {
        switch (opt)
        {
        case 'V':
            cout << "vshell 1.0.0" << endl;
            return 1;
            break;
        case 'd':
            format = FORMAT_MASK(format | DATE_MASK);
            break;
        case 'f':
            format = FORMAT_MASK(format | FILENAME_MASK);
            break;
        case 'l':
            format = FORMAT_MASK(format | LINE_MASK);
            break;
        case 'i':
            input_file = string(optarg);
            break;
        case 'o':
            output_file = string(optarg);
            break;
        case 't':
            format = FORMAT_MASK(format | TIME_MAKE);
            break;
        default:
            cout << "???" << endl;
            return -1;
            break;
        }
    }
    return 0;
}

} // namespace vshell
