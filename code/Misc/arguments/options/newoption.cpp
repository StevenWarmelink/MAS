#include "options.ih"

void Options::newOption(int optChar, char const *optArg)
{
    Option **tmp = new Option *[d_size + 1];
    memcpy(tmp, d_option, d_size * sizeof(Option *));

    delete[] d_option;
    d_option = tmp;

    d_option[d_size++] = new Option(optChar, optArg);
}
