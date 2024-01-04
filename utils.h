#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>

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

void writeToArchive (std::filesystem::path archive, std::filesystem::path dir)
{
    struct archive *a;
    struct archive_entry *entry;
    struct stat st;
    char buff[8192];
    int len;
    int fd;

    a = archive_write_new();
    archive_write_add_filter_gzip(a);
    archive_write_set_format_pax_restricted(a);
    archive_write_open_filename(a, archive.c_str());


    for (auto file: std::filesystem::directory_iterator (dir)) {
        stat(file.path().c_str(), &st);

        entry = archive_entry_new();

        if (file.is_directory())
            archive_entry_set_pathname(entry, (file.path() / "").c_str()); // append separator to mark as directory
        else
            archive_entry_set_pathname(entry, file.path().c_str());
        archive_entry_set_size(entry, st.st_size);
        archive_entry_set_filetype(entry, AE_IFREG);
        archive_entry_set_perm(entry, 0644);

        archive_write_header(a, entry);

        if(file.is_directory())
            std::cout << archive_entry_pathname(entry) << std::endl;

        fd = open(file.path().c_str(), O_RDONLY);
        len = read(fd, buff, sizeof(buff));

        while ( len > 0 ) {
            archive_write_data(a, buff, len);
            len = read(fd, buff, sizeof(buff));
        }

        close(fd);
        archive_entry_free(entry);
    }

    archive_write_close(a);
    archive_write_free(a);
}

int copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
#if ARCHIVE_VERSION >= 3000000
    int64_t offset;
#else
    off_t offset;
#endif

    while(true) {
        r = archive_read_data_block(ar, &buff, &size, &offset);

        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r != ARCHIVE_OK)
            return (r);

        r = archive_write_data_block(aw, buff, size, offset);

        if (r != ARCHIVE_OK) {
            std::cerr << "archive_write_data_block(): " << archive_error_string(aw) << std::endl;
            return (r);
        }
    }
}

void readFromArchive (std::filesystem::path archive, std::filesystem::path dir)
{
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int r;
    int flags = ARCHIVE_EXTRACT_TIME;

    a = archive_read_new();
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);

    archive_read_support_format_tar(a);

    if (archive_read_open_filename(a, archive.c_str(), 10240)) {
        std::cerr  << "archive_read_open_file() failed: " << archive_error_string(a) << std::endl;
            return;
    }

    while (true) {
        r = archive_read_next_header(a, &entry);


        if (r == ARCHIVE_EOF)
            break;
        if (r != ARCHIVE_OK)
            std::cerr << "archive_read_next_header() failed: " << archive_error_string(a) << std::endl;

        auto p = std::filesystem::path (archive_entry_pathname(entry));

        if (p.parent_path() != dir)
            continue;

        r = archive_write_header(ext, entry);

        if (r != ARCHIVE_OK)
            std::cerr << "archive_write_header(): " << archive_error_string(ext) << std::endl;

        else {
            copy_data(a, ext);
            r = archive_write_finish_entry(ext);

            if (r != ARCHIVE_OK) {
                std::cerr << "archive_write_finish_entry()" << archive_error_string(ext) << std::endl;
                break;
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);
}

std::vector<std::filesystem::path> readDirectories (std::filesystem::path archive)
{
    std::vector<std::filesystem::path> directories;

    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    r = archive_read_open_filename(a, archive.c_str(), 10240);
    if (r != ARCHIVE_OK)
        exit(1);

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        if(!std::filesystem::path(archive_entry_pathname(entry)).has_filename()) // no filename means directory
            directories.push_back(archive_entry_pathname(entry));

        archive_read_data_skip(a);
    }

    r = archive_read_free(a);

    if (r != ARCHIVE_OK)
        exit(1);

    return directories;
}

} // namespace randomly

#endif // UTILS_H
