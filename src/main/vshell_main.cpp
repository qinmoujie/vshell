
#include "../shell_parser/shell_parser.h"
#include "../format/out_format.h"

using namespace std;

int main(int argc, char *args[])
{
    string inf_pname, outfilename, filename;
    inf_pname = string(args[1]);
    outfilename = string(args[2]);
    ifstream inf;
    ofstream ouf;
    inf.open(inf_pname);
    if (!inf)
    {
        cout << "Open error: " << inf_pname << endl;
        return 1;
    }
    ouf.open(outfilename);
    if (!ouf)
    {
        cout << "Open error: " << outfilename << endl;
        return 1;
    }

    using namespace vshell;
    vector<string> v_file;
    string buf;
    while (getline(inf, buf))
        v_file.emplace_back(std::move(buf));

    vshell::Shell_parser::outfile_type outf;
    vshell::Shell_parser sh_parser(v_file);
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

    // for (const auto &of : outf)
    // {
    //     cout << to_string(of.index) << "|";
    //     if (of.mode == NORMAL)
    //     {
    //         cout << "NORMAL";
    //     }
    //     else if (of.mode == V_FIRST)
    //     {
    //         cout << "V_FIRST";
    //     }
    //     else if (of.mode == V_SECOND)
    //     {
    //         cout << "V_SECOND";
    //     }
    //     cout << ": " << of.line << endl;
    // }
    
    vshell::FORMAT_MASK format = vshell::NO_FORMAT_MASK;
    format = FORMAT_MASK(format | DATE_MASK | TIME_MAKE | FILENAME_MASK | LINE_MASK | VAR_VALUE_SHOW_MASK);
    vshell::Out_format oformat(format, inf_pname);
    for (const auto &of : outf)
    {
        cout << oformat.format_cmd(of.index, of.mode, of.line) << endl;
    }

    inf.close();
    ouf.close();
    return 0;
}