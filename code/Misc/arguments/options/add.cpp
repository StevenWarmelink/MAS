#include "options.ih"

void Options::add(int optChar, char const *optVal)
{
    ++d_nOptions;

    Option *option = find(optChar);

    if (option == 0)
        newOption(optChar, optVal);
    else
        option->add(optVal);
    
}
