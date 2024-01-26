#include "backup.h"

#include "utils.h"
#include "archiveFunctions.h"

#include <iostream>
#include <filesystem>

using namespace std;

namespace randomly
{

void backupDir (const filesystem::path &dir, const filesystem::path &backupFile)
{
    cout << "backup "
         << (Blue | Bold)
         << dir
         << Default << endl;

    // add directory to tar file
    writeToArchive (backupFile, dir);
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

    // clear previous backup
    cout << "clearing previous backup" << endl;

    filesystem::remove(backupFile);

    // backup files in the world root
    backupDir (worldFixed, backupFile);

    // backup all the directories from the main world
    for (auto& p : filesystem::recursive_directory_iterator (world))
        if (p.is_directory ()) {
            cout << "when calling: " << p << endl;
            backupDir (p, backupFile);
        }

    cout << (Red | Bold)
         << "backup done! "
         << Default
         << "backup file: "
         << (Blue | Bold)
         << "\033]8;;file://" << backupFile.parent_path().string() << "\033\\" << backupFile.string() << "\033]8;;\033\\"
         << Default << endl;
}

} // namespace randomly
