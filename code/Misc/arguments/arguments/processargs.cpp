#include "arguments.ih"

void  Arguments::processArgs(int argc, char **argv)
{
    for (char **arg = argv + optind, **end = argv + argc; arg != end; ++arg)
        addArg(*arg);
}
