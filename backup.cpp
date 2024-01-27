#include "backup.h"

#include "utils.h"
#include "archiveFunctions.h"

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;

namespace randomly
{

void backupDir (const filesystem::path &dir, archive *a)
{
    cout << "backup "
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

    cout << (Red | Bold)
         << "backup done! "
         << Default
         << "backup file: "
         << (Blue | Bold)
         << "\033]8;;file://" << backupFile.parent_path().string() << "\033\\" << backupFile.string() << "\033]8;;\033\\"
         << Default << endl;
}

} // namespace randomly
