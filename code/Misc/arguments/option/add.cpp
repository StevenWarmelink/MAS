#include "option.ih"

void Option::add(char const *optVal)
{
    ++d_count;

    if (not optVal)
        return;

    string **tmp = new string *[d_size + 1];
    memcpy(tmp, d_optVal, d_size * sizeof(string *));

    delete[] d_optVal;
    d_optVal = tmp;
    
    d_optVal[d_size++] = new string(optVal);
}
