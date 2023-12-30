#ifndef FANCYCOLORING_H
#define FANCYCOLORING_H

namespace randomly
{

enum Color
{
    Default =  0,
    Black   = 30,
    Red     = 31,
    Green   = 32,
    Yellow  = 33,
    Blue    = 34,
    Magenta = 35,
    Cyan    = 36,
    White   = 37,
};

enum Style
{
    Bold        = 1,
    Underline   = 4,
};

const char *colorCode (Color c)
{
    switch (c)
    {
    case Default:   return "\033[00m";
    case Black:     return "\033[30m";
    case Red:       return "\033[31m";
    case Green:     return "\033[32m";
    case Yellow:    return "\033[33m";
    case Blue:      return "\033[34m";
    case Magenta:   return "\033[35m";
    case Cyan:      return "\033[36m";
    case White:     return "\033[37m";
    }

    return nullptr; // error
}

const char *styleCode (Style s)
{
    switch (s)
    {
    case Bold:      return "\033[01m";
    case Underline: return "\033[04m";
    }

    return nullptr; // error
}

} // namespace randomly

#endif // FANCYCOLORING_H
