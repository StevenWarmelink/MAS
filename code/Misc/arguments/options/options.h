#ifndef INCLUDED_OPTIONS_
#define INCLUDED_OPTIONS_

#include <iosfwd>
#include "../option/option.h"

class Options
{
    size_t d_size = 0;
    Option **d_option = 0;

    size_t d_nOptions = 0;

    public:
        Options() = default;
        Options(Options const &other) = delete;

        ~Options();

        Options &operator=(Options const &other) = delete;
        Options &operator=(Options &&tmp);

        void add(int optChar, char const *optArg = 0);

        size_t size() const;
        size_t option(int optChar) const;                       // 1
        size_t option(std::string const &options) const;        // 2
        size_t option(std::string *value, size_t idx, int optChar) const; // 3

    private:
        void swap(Options &other);
        Option *find(int optChar) const;                        // backdoor
        void newOption(int optChar, char const *optArg);

};
        
inline size_t Options::size() const
{
    return d_nOptions;
}

#endif
