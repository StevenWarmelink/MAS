#include "arguments.ih"

char const *Arguments::arg(size_t idx) const
{
    return idx >= d_nArgs ? 0 : d_arg[idx]->c_str();
}
