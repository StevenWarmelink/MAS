#include "option.ih"

Option::~Option()
{
    if (d_optVal == 0)
        return;

    for (; d_size--; )
        delete d_optVal[d_size];

    delete[] d_optVal;
}
