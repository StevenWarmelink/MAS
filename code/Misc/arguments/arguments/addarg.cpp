#include "arguments.ih"

void Arguments::addArg(char const *arg)
{
    extendArgs();
    d_arg[d_nArgs++] = new string(arg);
}
