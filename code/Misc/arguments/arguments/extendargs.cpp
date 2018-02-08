#include "arguments.ih"

void Arguments::extendArgs()
{
    string **tmp = new string *[d_nArgs + 1];
    memcpy(tmp, d_arg, d_nArgs * sizeof(string *));
    delete[] d_arg;
    d_arg = tmp;
}
