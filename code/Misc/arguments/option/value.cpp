#include "option.ih"

size_t Option::value(std::string *argVal, size_t idx) const
{
    if (idx >= d_size)
        return 0;

    if (argVal)
        *argVal = *d_optVal[idx];

    return d_count;
}
