#include <iomanip>
#include <stdlib.h>

#include "command.h"

using namespace std;

namespace vshell
{
bool vshell_error::error(const string &error_messages)
{
    std::cerr << "vshell: error: " + error_messages << std::endl;
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

void print_help()
{
    cout.flags(ios::left);
    const size_t max_long_option_length = 25;
    cout
        << "Usage: vshell [OPTIONS] <inputs>"
        << "\n\n"
        << "OPTIONS:"
        << "\n"
        << "    -V " << setw(max_long_option_length) << " --version"
        << "Show version number"
        << "\n"
        << "    -d " << setw(max_long_option_length) << " --date"
        << "Add date of run command in out shell file"
        << "\n"
        << "    -f " << setw(max_long_option_length) << " --filename"
        << "For each command Add the filename which it belong to in out shell file"
        << "\n"
        << "       " << setw(max_long_option_length) << " --force"
        << "Force replacement of multiple original files"
        << "\n"
        << "    -h " << setw(max_long_option_length) << " --help"
        << "Print this help text"
        << "\n"
        << "    -l " << setw(max_long_option_length) << " --line"
        << "Add line number of command in input file to out shell file"
        << "\n"
        << "    -o " << setw(max_long_option_length) << " --output-file <file>"
        << "Output shell file name from vshell"
        << "\n"
        << "    -r " << setw(max_long_option_length) << " --run"
        << "Run out shell script or commands from vshell"
        << "\n"
        << "    -s " << setw(max_long_option_length) << " --string"
        << "Specify a string as inputs"
        << "\n"
        << "    -t " << setw(max_long_option_length) << " --time"
        << "Add time of run command in out shell file"
        << "\n"
        << endl;
}

enum OPTIONS_NUMBER
{
    FORCE_OPTION,
};

int parse_cmdline_options(int argc, char **argv, vector<string> &input_files,
                          string &output_file, FORMAT_MASK &format)
{
    int opt;
    static struct option long_options[] =
        {
            {"version", no_argument, NULL, 'V'},
            {"date", no_argument, NULL, 'd'},
            {"filename", no_argument, NULL, 'f'},
            {"force", no_argument, NULL, FORCE_OPTION},
            {"help", no_argument, NULL, 'h'},
            {"line", no_argument, NULL, 'l'},
            {"output-file", required_argument, NULL, 'o'},
            {"run", no_argument, NULL, 'r'},
            {"string", no_argument, NULL, 's'},
            {"time", no_argument, NULL, 't'},
            {0, 0, 0, 0},
        };
    for (; (opt = getopt_long(argc, argv, "Vdfhlo:rs:t", long_options, NULL)) != -1;)
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
        case 'h':
            print_help();
            return 1;
            break;
        case 'l':
            format = FORMAT_MASK(format | LINE_MASK);
            break;
        case 'o':
            output_file = string(optarg);
            break;
        // case 'r':
        //     output_file = string(optarg);
        //     break;
        // case 's':
        //     output_file = string(optarg);
        //     break;
        case 't':
            format = FORMAT_MASK(format | TIME_MAKE);
            break;
        case FORCE_OPTION:
            format = FORMAT_MASK(format | FORCE_MAKE);
            break;
        default:
            return -1;
            break;
        }
    }
    for (; optind < argc;)
    {
        input_files.push_back(argv[optind++]);
    }
    return 0;
}

bool shell_run(const std::string &cmdstrs)
{
    int maxline = 100;
    char line[maxline];
    FILE *fpin;
    int ret;

    if ((fpin = popen(cmdstrs.c_str(), "r")) == NULL)
    {
        return vshell_error::error("popen error");
    }
    for (;;)
    {
        fputs("vshell>>> ", stdout);
        fflush(stdout);
        if (fgets(line, maxline, fpin) == NULL)
            break;
        if (fputs(line, stdout) == EOF)
        {
            return vshell_error::error("fputs error\n");
        }
    }
    if ((ret = pclose(fpin)) == -1)
    {
        return vshell_error::error("pclose error\n");
    }
    return ret;
}

} // namespace vshell
