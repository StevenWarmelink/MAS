#include "options.ih"

void Options::swap(Options &other)
{
    char buffer[sizeof(Options)];
    memcpy(buffer, this,   sizeof(Options));
    memcpy(this,   &other, sizeof(Options));
    memcpy(&other, buffer, sizeof(Options));
}
