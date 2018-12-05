#include "../shell_parser/shell_parser.h"
#include "../format/out_format.h"
#include "../../lib/command.h"

using namespace std;
using namespace vshell;

bool vshell_file(const string &input_file,
                 const string &output_file,
                 vshell::FORMAT_MASK format)
{
    vector<string> v_file;
    if (vshell::trans_file_to_vector(input_file, v_file) == false)
        return false;
    vshell::Shell_parser::outfile_type outf;
    vshell::Shell_parser sh_parser(v_file, input_file);
    try
    {
        if (sh_parser.parser(outf) == false)
            return false;
    }
    catch (...)
    {
        vshell::vshell_error::error("unknown error");
        return false;
    }

    vshell::Out_format oformat(format, input_file);
    if (oformat.format_out(output_file, outf) == false)
        return false;
    return true;
}

bool vshell_files(const vector<string> &input_files,
                  const vector<string> &output_files,
                  vshell::FORMAT_MASK format)
{
    if (input_files.size() != output_files.size())
        return vshell_error::error("the number of input files is not equal output files");
    for (size_t i = 0; i != input_files.size(); ++i)
    {
        if (vshell_file(input_files[i], output_files[i], format) == false)
            return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    vector<string> input_files;
    string output_file = "./a.sh";
    vshell::FORMAT_MASK format = vshell::VAR_VALUE_SHOW_MASK;
    int cmd_res = parse_cmdline_options(argc, argv, input_files, output_file, format);
    if (cmd_res == 0)
    {
        if (Out_format::is_force_option(format) == true)
        {
            vector<string> output_files(input_files);
            if (vshell_files(input_files, output_files, format) == false)
                return 1;
            return 0;
        }
        else
        {
            if (input_files.empty())
            {
                vshell_error::error("no input files");
                return 1;
            }
            else if (input_files.size() > 1)
            {
                vshell_error::error("too many input files");
                return 1;
            }
            else
            {
                if (vshell_file(input_files.front(), output_file, format) == false)
                    return 1;
                if (Out_format::is_run_option(format) == true)
                    return run_shell(trans_relative_path_to_abs(output_file));
                return 0;
            }
        }
    }

    return cmd_res == 1 ? 0 : 1;
}