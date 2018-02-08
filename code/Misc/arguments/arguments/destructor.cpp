#include "arguments.ih"

Arguments::~Arguments()
{
    if (d_valid && s_available)
        s_available = false;

    clear();
}
