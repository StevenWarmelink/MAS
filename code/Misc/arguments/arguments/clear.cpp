#include "arguments.ih"

void Arguments::clear()
{
    for (; d_nArgs--; )
        delete d_arg[d_nArgs];

    delete[] d_arg;
    d_arg = 0;
    d_nArgs = 0;

    d_basename.clear();

    d_valid = false;

    d_options = Options();
}
