#include "options.ih"

size_t Options::option(std::string const &options) const
{
    size_t ret = 0;

    for (auto optChar: options)
        ret += option(optChar);

    return ret;
}
