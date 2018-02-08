#include "option.ih"

Option::Option(int option, char const *optVal)
:
    d_option(option)
{
    add(optVal);
}
