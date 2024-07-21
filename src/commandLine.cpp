#include "commandLine.h"

namespace randomly
{

bool randomly::Options::operator=(const char *rhs) {
    backupSet = true;

    if (std::strcmp (rhs, "backup") == 0) {
        backup = true;
        return true;
    }

    else if (std::strcmp (rhs, "restore") == 0) {
        backup = false;
        return true;
    }

    else {
        std::cerr << (Red | Underline)
                  << "no valid tool specified!" << std::endl;

        exit (EXIT_FAILURE);

        return false; // only to silence potential compiler warnings
    }
}

void printHelp()
{
    std::cout << "A simple tool to backup minecraft worlds and restore their backups"   << std::endl
              << "Usage: "                                                              << std::endl
              << "\t[-w <world name>]\t\t set world name (default: ask user)"           << std::endl
              << "\t[-p <directory>]\t\t path to saves directory (default: current)"    << std::endl
              << "\t[-t <backup|restore>]\t\t tool to use. (default: backup)"           << std::endl
              << "\t[--help]\t\t\t print this help"                                     << std::endl;
}

Options parseCommandLine(int argc, char **argv)
{
    Options parsed;

    for (int i = 1; i < argc; ++i) {
        if (strcmp (argv[i], "--help") == 0 || strcmp (argv[i], "-h") == 0) {
            printHelp();
            exit(0);
        }

        if(evaluateArgument (argc, argv, i, "-p", "no directory specified!", parsed.savesDirectory))
            continue;

        if (evaluateArgument (argc, argv, i, "-w", "no world name specified!", parsed.world))
            continue;

        if (evaluateArgument (argc, argv, i, "-t", "no tool specified!", parsed))
            continue;
    }

    return parsed;
}

} // namespace randomly
