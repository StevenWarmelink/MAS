#include "arguments.ih"

bool Arguments::processOptions(int argc, char **argv, char const *optstring)
{
    opterr = 0;     // no error messages by getopt

    string opts(*optstring == ':' ? "" : ":");  // ensure initial char is ':'
    opts += optstring;
    
    while (true)
    {
        int opt = getopt(argc, argv, opts.c_str());

        switch (opt)
        {
            case -1:
            return true;

            case '?':
                cerr << "Option `-" << static_cast<char>(optopt) << 
                                                    "' not supported\n";
            return false;

            case ':':
                cerr << "Option `-" << static_cast<char>(optopt) << 
                                                    "': missing argument\n";
            return false;
            
            default:
                d_options.add(opt, optarg); // 0-pointer if no arg. value
            break;
        }
    }
}
