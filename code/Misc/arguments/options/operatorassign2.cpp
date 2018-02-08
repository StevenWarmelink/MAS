#include "options.ih"

Options &Options::operator=(Options &&tmp)
{
    swap(tmp);
    return *this;
}
