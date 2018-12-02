
#include "../shell_parser/shell_parser.h"
#include "../format/out_format.h"
#include "../../lib/command.h"

using namespace std;

int main(int argc, char *args[])
{
    using namespace vshell;
    string input_file, output_file = "a.sh";
    vshell::FORMAT_MASK format = vshell::VAR_VALUE_SHOW_MASK;
    int cmd_res;
    if ((cmd_res = parse_cmdline_options(argc, args, input_file, output_file, format)) == -1)
        return 1;
    else if (cmd_res == 1)
        return 0;
    vector<string> v_file;
    if (vshell::trans_file_to_vector(input_file, v_file) == false)
        return 1;
    vshell::Shell_parser::outfile_type outf;
    vshell::Shell_parser sh_parser(v_file, input_file);
    try
    {
        if (sh_parser.parser(outf) == false)
            return 1;
    }
    catch (...)
    {
        vshell::vshell_error::error("unknown error");
        return 2;
    }

    vshell::Out_format oformat(format, input_file);
    if (oformat.format_out(output_file, outf) == false)
        return 1;
    return 0;
}