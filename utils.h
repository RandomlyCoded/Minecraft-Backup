#ifndef UTILS_H
#define UTILS_H

#include <assert.h>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

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

struct Options {
    bool backup = true;
    std::string world;
    std::filesystem::path savesDirectory = ".";

    /// used to assign the tool when parsing command line arguments
    bool operator= (const char *rhs) {
        if (std::strcmp (rhs, "backup") == 0) {
            backup = true;
            return true;
        }

        else if (std::strcmp (rhs, "restore") == 0) {
            backup = false;
            return true;
        }

        else {
            std::cerr << colorCode (Red) << styleCode (Underline)
                      << "no valid tool specified!" << std::endl;

            exit (EXIT_FAILURE);

            return false; // only to silence potential compiler warnings
        }
    }
} options;

void printHelp()
{
    std::cout << "A simple tool to backup minecraft worlds and restore their backups"   << std::endl
              << "Usage: "                                                              << std::endl
              << "\t[-w <world name>]\t\t set world name (default: ask user)"           << std::endl
              << "\t[-p <directory>]\t\t path to saves directory (default: current)"    << std::endl
              << "\t[-t <tool>]\t\t\t tool to use. (default: backup)"                    << std::endl
              << "\t           \t\t\t - valid values: \"backup\", \"restore\""                           << std::endl
              << "\t[--help]\t\t\t print this help"                                     << std::endl;
}

template <typename T>
bool evaluateArgument (int argc, char **argv, int &index, const char *test, const char *errorMsg, T &option)
{
    if (std::strcmp (argv[index], test) == 0) {
        if (!(index + 1 < argc)) {
            std::cerr << colorCode (Red) << styleCode (Underline)
                      << errorMsg << std::endl;

            exit (EXIT_FAILURE);
        }

        option = argv[index + 1];

        ++index; // skip next argument;

        return true;
    }
    return false;
}

void parseCommandLine(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp (argv[i], "--help") == 0 || std::strcmp (argv[i], "-h") == 0) {
            printHelp();
            exit(0);
        }

        if(evaluateArgument (argc, argv, i, "-p", "no directory specified!", options.savesDirectory))
            continue;

        if (evaluateArgument (argc, argv, i, "-w", "no world name specified!", options.world))
            continue;

        if (evaluateArgument (argc, argv, i, "-t", "no tool specified!", options))
            continue;
    }
}

} // namespace randomly

#endif // UTILS_H
