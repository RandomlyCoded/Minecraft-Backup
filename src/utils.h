#ifndef RANDOMLY_UTILS_H
#define RANDOMLY_UTILS_H

#include <iostream>
#include <random>
#include <string>

namespace randomly
{

enum Style : short
{
    Default   = 0,
    Black     = 1,
    Red       = 2,
    Green     = 3,
    Yellow    = 4,
    Blue      = 5,
    Magenta   = 6,
    Cyan      = 7,
    White     = 8,
    ColorMask = 15,

    Bold         = 16,
    Underline    = 32,
    UnderlineOff = 64,

    ClearLine = 1024,
    LineUp    = 1025
};

static Style operator|(Style lhs, Style rhs)
{
    return Style (int(lhs) | int(rhs));
}

static const std::string styleCode (Style s)
{
    if (s == ClearLine)
        return "\033[2K";

    if (s == LineUp)
        return "\033[A";

    std::string data;

    switch (s & ColorMask)
    {
    case Default: data = "\033[00m"; break;
    case Black:   data = "\033[30m"; break;
    case Red:     data = "\033[31m"; break;
    case Green:   data = "\033[32m"; break;
    case Yellow:  data = "\033[33m"; break;
    case Blue:    data = "\033[34m"; break;
    case Magenta: data = "\033[35m"; break;
    case Cyan:    data = "\033[36m"; break;
    case White:   data = "\033[37m"; break;
    }

    if (s & Bold)
        data += "\033[01m";

    if (s & Underline)
        data += "\033[04m";
    if (s & UnderlineOff)
        data += "\033[24m";

    return data;
}

static std::ostream &operator<<(std::ostream &os, Style s)
{
    return os << styleCode(s);
}

static char *generateRandomExtension()
{
    // 16 characters + NULL byte
    auto data = new char[17];
    data[16] = 0;

    for (int i = 0; i < 16; ++i) {
        auto rnd = rand();
        data[i] = (rnd % 26 + 65) | (rnd & 32);

        // mod 26 normalizes the result into "normal" characters, adding 65 moves it into the ASCII character area
        // ORing with (rnd & 32) might add another 32 which shifts the character into lowercase
    }

    return data;
}

} // namespace randomly

#endif // RANDOMLY_UTILS_H
