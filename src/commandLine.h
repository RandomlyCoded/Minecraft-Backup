#ifndef RANDOMLY_COMMANDLINE_H
#define RANDOMLY_COMMANDLINE_H

#include "utils.h"

#include <cstring>
#include <filesystem>
#include <iostream>

namespace randomly
{

struct Options {
    bool backup = true;
    bool backupSet = false;

    std::string world;
    std::filesystem::path savesDirectory = std::filesystem::current_path();

    bool useGUI = false;

    /// used to assign the tool when parsing command line arguments
    bool operator= (const char *rhs);
};

template <typename T>
bool evaluateArgument (int argc, char **argv, int &index, const char *test, const char *errorMsg, T &option)
{
    if (std::strcmp (argv[index], test) == 0) {
        if (!(index + 1 < argc)) {
            std::cerr << (Red | Underline)
                      << errorMsg << std::endl;

            exit (EXIT_FAILURE);
        }

        option = argv[index + 1];

        ++index; // skip next argument since it's the value

        return true;
    }

    return false;
}

void printHelp();

Options parseCommandLine(int argc, char **argv);

} // namespace randomly

#endif // RANDOMLY_COMMANDLINE_H
