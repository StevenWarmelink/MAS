#include "options.ih"

size_t Options::option(int optChar) const
{
    Option *option = find(optChar);

    return option == 0 ? 0 : option->size();
}
