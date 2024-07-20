#include "backup.h"

#include "utils.h"
#include "archiveFunctions.h"

#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <sys/ioctl.h>

using namespace std;

namespace randomly
{

namespace
{

int getScreenWidth()
{
    auto info = new winsize;

    ioctl(0, TIOCGWINSZ, info);

    // gotta do it this way due to memory leaks
    int wsCol = info->ws_col;
    delete info;

    return wsCol;
}

struct HumanReadable
{
    std::uintmax_t size{};
    bool showBytes = true;

private:
    friend std::ostream& operator<<(std::ostream& os, HumanReadable hr)
    {
        int o{};
        double mantissa = hr.size;
        for (; mantissa >= 1024.; mantissa /= 1024., ++o);

        os << std::setw(5) << std::ceil(mantissa * 10.) / 10. << std::setw(0) << " " << "BKMGTPE"[o];

        if(hr.showBytes)
            return o ? os << "B (" << hr.size << ')' : os;

        return os << "B";
    }
};

}

/***********************
 * Backup functionality *
  **********************/

Backup::Backup(Options &options, GUIManager *guiMgr)
    : m_options(options)
    , m_guiMgr(guiMgr)
{}

void Backup::backupSave(std::string world)
{
    if (m_options.world.empty() || !filesystem::exists(m_options.savesDirectory / m_options.world)) {
        cerr << (Red | Bold | Underline)
             << "world does not exist, here are all available ones: " << endl
             << Default;

        for (auto& p : filesystem::directory_iterator (m_options.savesDirectory))
            if (p.is_directory ())
                cout << p.path().filename().string() << endl;

        exit(1);
    }

    cout << "starting to backup "
         << (Magenta | Bold)
         << world
         << Default << endl;

    const auto backupFile = m_options.savesDirectory.parent_path() / "backups" / (world + "-backup.tar.xz");

    // backup files in the world root
    auto ext = generateRandomExtension();

    auto archive = openWriteArchive(backupFile.string() + ext);

    backupDir (world, archive);

    // backup all the directories from the main world
    for (auto& p : filesystem::recursive_directory_iterator (world))
        if (p.is_directory ()) {
            backupDir (p, archive);
        }

    closeWriteArchive(archive);

    filesystem::rename(backupFile.string() + ext, backupFile);

    cout << ClearLine << (Red | Bold)
         << "backup done! "
         << Default
         << "backup file: "
         << (Blue | Bold)
         << "\033]8;;file://" << backupFile.parent_path().string() << "\033\\" << backupFile.string() << "\033]8;;\033\\"
         << Default
         << " (" << HumanReadable{filesystem::file_size(backupFile), false} << ")" << endl;
}

void Backup::backupDir (const filesystem::path &dir, archive *a)
{
    m_currentlyInProgress = dir;

    cout << ClearLine << "backup "
         << (Blue | Bold)
         << dir
         << Default << endl;

    // add directory to tar file
    writeToArchive (a, dir, this);
}

/************************
 * Restore functionality *
  ***********************/

void Backup::restoreSave (string world)
{
    const auto backupFile = m_options.savesDirectory.parent_path() / "backups" / (world + "-backup.tar.xz");

    if (m_options.world.empty() || !filesystem::exists(backupFile)) {
        cerr << (Red | Bold | Underline)
             << "backup does not exist, here are all available ones: " << endl
             << Default;

        for (auto& p : filesystem::directory_iterator (m_options.savesDirectory.parent_path() / "backups"))
            if (p.is_regular_file()) {
                auto name = p.path().string();

                if (name.ends_with("-backup.tar.xz")) {
                    auto path = p.path();

                    auto filename = path.filename().string();

                    cout << filename.substr(0, filename.length() - 14) << endl; // 14 is the length of "-backup.tar.xz
                }
            }

        exit(1);
    }

    cout << "restoring world "
         << (Magenta | Bold)
         << world
         << Default << endl;

    auto directories = readDirectories(backupFile);

    // we want to restore into a temporary directory, so we prepend a random prefix to the directories
    const auto prefix = generateRandomExtension();

    for (auto &dir: directories) {
        dir = prefix + dir.string();
    }

    // restore all the directories from the backup file
    for (auto& p: directories) {
        restoreDir (p, backupFile, prefix);
    }

    // we first need to delete the old world
    filesystem::remove_all(world);

    // and now we should be able to just move the root, right?
    filesystem::rename(prefix + world, world);
}

void Backup::restoreDir (const filesystem::path &dir, const filesystem::path &backupFile, const char *prefix)
{
    // the directories start with 16 random characters we don't want to display
    auto name = dir.string();
    name = name.substr(16);

    m_currentlyInProgress = name;

    cout << "restoring "
         << (Blue | Bold)
         << (dir.empty () ? "world root" : name)
         << Default << endl;

    // create directory
    filesystem::create_directory(dir);

    // restore content
    readFromArchive(backupFile, dir, prefix, this);
}

/**************
 * UI handling *
  *************/

void Backup::updateUiState(int filesTotal, int filesProcessed, const filesystem::path &currentFile)
{
    if (m_options.useGUI)
        setGUIProgressState(filesTotal, filesProcessed, currentFile);
    else
        setTerminalProgressBar(filesTotal, filesProcessed, currentFile);
}

void Backup::setTerminalProgressBar(int filesTotal, int filesProcessed, const filesystem::path &currentFile)
{
    if (filesProcessed < 0) // only marking current directory
        return;

    const auto width = getScreenWidth() - 4 - 10;

    // clear current line (progress bar)
    cout << ClearLine;

    const auto amt = width * filesProcessed / filesTotal;

    cout << Green << "  ";

    for (int i = 0; i < amt; ++i)
        cout << '#';

    cout << (Red | Bold);

    for (int i = amt; i < width; ++i)
        cout << '_';

    cout << Default;

    cout << " " << setw(4) << filesProcessed << "/" << setw(4) << filesTotal << endl;

    // clear current line, aka the filename line
    cout << ClearLine;
    cout << currentFile.filename().string();

    cout << '\r';
    cout  << LineUp;
    cout.flush();
}

void Backup::setGUIProgressState(int filesTotal, int filesProcessed, const filesystem::path &currentFile)
{
    if (!m_guiMgr) {
        cerr << (Red | Bold) << "ERROR: no GUI Manager provided, but GUI requested. Terminating." << Default << endl;
        exit(EXIT_FAILURE);
    }

    m_guiMgr->update(filesTotal, filesProcessed, currentFile, m_currentlyInProgress);
}

} // namespace randomly
