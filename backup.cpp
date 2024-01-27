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

    return info->ws_col;
}

}

void backupDir (const filesystem::path &dir, archive *a)
{
    cout << ClearLine << "backup "
         << (Blue | Bold)
         << dir
         << Default << endl;

    // add directory to tar file
    writeToArchive (a, dir);
}

void backupSave (string world)
{
    cout << "starting to backup "
         << (Magenta | Bold)
         << world
         << Default << endl;

    // we need to replace every whitespace with "\ "
    string worldFixed;

    for (auto c: world) {
        if (c == ' ')
            worldFixed += '\\';
        worldFixed += c;
    }

    const auto backupFile = filesystem::current_path().parent_path() / "backups" / (worldFixed + "-backup.tar.xz");

    // backup files in the world root
    auto ext = generateRandomExtension();

    auto archive = openWriteArchive(backupFile.string() + ext);

    backupDir (worldFixed, archive);

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
         << Default << endl;
}

void setProgressBar(int filesTotal, int filesProcessed)
{
    const auto width = getScreenWidth() - 4;
    // move up one line and clear previous line
    cout << "\r" << "\033[2K";

    const auto amt = width * filesProcessed / filesTotal;

    cout << Green;

    for (int i = 0; i < amt; ++i)
        cout << '#';

    cout << (Red | Bold);

    for (int i = amt; i < width; ++i)
        cout << '_';

    cout << Default;

    cout << '\r';
    cout.flush();
}

} // namespace randomly
