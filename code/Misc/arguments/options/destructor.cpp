#include "options.ih"

Options::~Options()
{
    if (d_option == 0)
        return;

    for (; d_size--; )
        delete d_option[d_size];

    delete[] d_option;
}
