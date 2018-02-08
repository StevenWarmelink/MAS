#include "arguments.ih"

Arguments::Arguments(int argc, char **argv, char const *optstring)
{
    if (s_available)
    {
        cerr << "Arguments object already available\n";
        return;
    }

    if (not processOptions(argc, argv, optstring))
        clear();                // return all allocated memory,
    else                        // setting coutners to 0
    {
        d_basename = ::basename(argv[0]);
        processArgs(argc, argv);

        s_available = true;
        d_valid = true;
    }
}




