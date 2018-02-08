#include "options.ih"

size_t Options::option(std::string *optVal, size_t idx, int optChar) const
{
    Option *option = find(optChar);

    return option == 0 ? 0 : option->value(optVal, idx);
}
