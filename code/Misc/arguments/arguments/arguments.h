#ifndef INCLUDED_ARGUMENTS_
#define INCLUDED_ARGUMENTS_

#include <iostream>
#include <string>

#include "../options/options.h"

class Arguments
{
    Options d_options;

    size_t d_nArgs = 0;
    std::string **d_arg = 0;

    std::string d_basename;
    bool d_valid = false;

    static bool s_available;            // keeps track of an available 
                                        // Arguments object

    public:
        Arguments(int argc, char **argv, char const *optstring);
        Arguments(Arguments const &other) = delete;

        ~Arguments();

        Arguments &operator=(Arguments const &other) = delete;

        std::string const &basename() const;

        size_t nArgs() const;
        char const *arg(size_t idx) const;

        size_t nOptions() const;
        size_t option(int optChar) const;
        size_t option(std::string const &options) const;
        size_t option(std::string *value, int optChar) const;
        size_t option(std::string *value, size_t idx, int optChar) const;
        bool valid() const;

    private:
        void clear();        
        bool processOptions(int argc, char **argv, char const *optstring);
        void processArgs(int argc, char **argv);
        void addArg(char const *arg);
        void extendArgs();
};  


inline bool Arguments::valid() const
{
    return d_valid;
}

inline std::string const &Arguments::basename() const
{
    return d_basename;
}
        
inline size_t Arguments::nArgs() const
{
    return d_nArgs;
}

inline size_t Arguments::nOptions() const
{
    return d_options.size();
}

inline size_t Arguments::option(int optChar) const
{
    return d_options.option(optChar);
}

inline size_t Arguments::option(std::string const &options) const
{
    return d_options.option(options);
}

inline size_t Arguments::option(std::string *value, int optChar) const
{
    return option(value, 0, optChar);
}

inline size_t Arguments::option(std::string *value, size_t idx, 
                                int optChar) const
{
    return d_options.option(value, idx, optChar);
}

#endif
