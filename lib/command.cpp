#include <iomanip>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>

#include "command.h"

#define MAXBUFSIZE 1024

using namespace std;

namespace vshell
{

void vshell_error::__print_error_message(const string &error_message)
{
    cerr << "vshell: error: " + error_message << endl;
}

bool vshell_error::error(const string &error_message)
{
    __print_error_message(error_message);
    return false;
}

int vshell_error::int_error(const string &error_message)
{
    __print_error_message(error_message);
    return 1;
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

istream &trans_istream_to_vector(istream &inf, vector<string> &v_file)
{
    vector<string> temp_file;
    string buf;
    for (; getline(inf, buf);)
        temp_file.emplace_back(std::move(buf));
    v_file.swap(temp_file);
    if (inf.bad())
        vshell_error::__print_error_message("failed to get vector file from istream");
    return inf;
}

bool trans_file_to_vector(const string &filename, vector<string> &v_file)
{
    ifstream inf(filename);
    if (!inf)
        return vshell_error::open_error(filename);
    bool ret = true;
    ret = (!trans_istream_to_vector(inf, v_file).bad());
    inf.close();
    return ret;
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
        << "       " << setw(max_long_option_length) << " --no-color"
        << "Not add color to out shell file"
        << "\n"
        << "    -o " << setw(max_long_option_length) << " --output-file <file>"
        << "Output shell file name from vshell"
        << "\n"
        << "    -p " << setw(max_long_option_length) << " --pipeline"
        << "Get inputs from pipeline or standard input"
        << "\n"
        << "    -r " << setw(max_long_option_length) << " --run"
        << "Run out shell script or commands from vshell"
        << "\n"
        << "    -t " << setw(max_long_option_length) << " --time"
        << "Add time of run command in out shell file"
        << "\n"
        << endl;
}

enum OPTIONS_NUMBER
{
    FORCE_OPTION,
    NO_COLOR_OPTION,
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
            {"no-color", no_argument, NULL, NO_COLOR_OPTION},
            {"output-file", required_argument, NULL, 'o'},
            {"pipeline", no_argument, NULL, 'p'},
            {"run", no_argument, NULL, 'r'},
            {"time", no_argument, NULL, 't'},
            {0, 0, 0, 0},
        };
    for (; (opt = getopt_long(argc, argv, "Vdfhlo:prt", long_options, NULL)) != -1;)
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
            Out_format::stdin_outfile = output_file;
            format = FORMAT_MASK(format | OUTPUTFILE_MASK);
            break;
        case 'p':
            format = FORMAT_MASK(format | PIPELINE_MASK);
            break;
        case 'r':
            format = FORMAT_MASK(format | RUN_MASK);
            break;
        case 't':
            format = FORMAT_MASK(format | TIME_MAKE);
            break;
        case FORCE_OPTION:
            format = FORMAT_MASK(format | FORCE_MASK);
            break;
        case NO_COLOR_OPTION:
            format = FORMAT_MASK(format | NOCOLOR_MASK);
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

string get_abs_workepath()
{
    char abs_workpath[MAXBUFSIZE];
    return getcwd(abs_workpath, MAXBUFSIZE);
}

string trans_relative_path_to_abs(const string &file)
{
    if (file.front() == '/')
        return file;
    return get_abs_workepath() + "/" + file;
}

int run_shell(const string &cmdstrs)
{
    int maxline = 10000;
    char line[maxline];
    FILE *fpin;
    int ret;

    if ((fpin = popen(cmdstrs.c_str(), "r")) == NULL)
    {
        return vshell_error::int_error("popen error");
    }
    for (;;)
    {
        fputs("\033[2;3;34m > \033[0m", stdout);
        fflush(stdout);
        if (fgets(line, maxline, fpin) == NULL)
            break;
        if (fputs(line, stdout) == EOF)
        {
            return vshell_error::int_error("fputs error\n");
        }
    }
    if ((ret = pclose(fpin)) == -1)
    {
        return vshell_error::int_error("pclose error\n");
    }
    return ret;
}

} // namespace vshell
