#ifndef INCLUDED_OPTION_
#define INCLUDED_OPTION_

#include <iosfwd>

class Option
{
    int d_option = 0;
    size_t d_count = 0;
    size_t d_size = 0;
    std::string **d_optVal = 0;

    public:
        Option(int option, char const *optVal = 0);
        Option(Option const &other) = delete;

        ~Option();

        Option &operator=(Option const &other) = delete;

        bool key(int optChar) const;
        size_t value(std::string *argVal, size_t idx) const;

        void add(char const *optVal);

        size_t size() const;
};

inline bool Option::key(int optChar) const
{
    return d_option == optChar;
}

inline size_t Option::size() const
{
    return d_count;
}
        

#endif
