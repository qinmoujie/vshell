
#include "../shell_parser/shell_parser.h"
#include "../format/out_format.h"

using namespace std;

int main(int argc, char *args[])
{
    using namespace vshell;
    string input_file_name(args[1]), output_file_name(args[2]);
    vector<string> v_file;
    if (vshell::trans_file_to_vector(input_file_name, v_file) == false)
        return 1;
    vshell::Shell_parser::outfile_type outf;
    vshell::Shell_parser sh_parser(v_file, input_file_name);
    try
    {
        if (sh_parser.parser(outf) == false)
            return 1;
    }
    catch (...)
    {
        cerr << "unknown error" << endl;
        return 2;
    }

    vshell::FORMAT_MASK format = vshell::NO_FORMAT_MASK;
    format = FORMAT_MASK(format | DATE_MASK | TIME_MAKE | FILENAME_MASK | LINE_MASK | VAR_VALUE_SHOW_MASK);
    vshell::Out_format oformat(format, input_file_name);
    if (oformat.format_out(output_file_name, outf) == false)
        return 1;
    return 0;
}