#include "options.ih"

Option *Options::find(int optChar) const
{
    for (size_t idx = 0; idx != d_size; ++idx)
    {
        Option *option = d_option[idx];
        if (option->key(optChar))
            return option;
    }

    return 0;
}
