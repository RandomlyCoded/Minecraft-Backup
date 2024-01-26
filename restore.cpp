#include "archiveFunctions.h"
#include "utils.h"

#include <iostream>

using namespace std;

namespace randomly
{

void restoreDir (const filesystem::path &dir, const filesystem::path &backupFile)
{
    cout << "restoring "
         << (Blue | Bold)
         << (dir.empty () ? "world root" : dir.relative_path ().c_str ())
         << Default << endl;

    // create directory
    filesystem::create_directory(dir);

    cout << "created dir" << endl;

    // restore content
    readFromArchive(backupFile, dir);

    cout << "finished reading" << endl;
}

void restoreSave (string world)
{
    cout << "restoring world "
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

    // clear save
    cout << "clearing previous save" << endl;
    filesystem::remove_all(worldFixed);

    const auto directories = readDirectories(backupFile);

    // restore all the directories from the backup file
    cout << directories.size() << endl;
    for (auto& p: directories) {
        std::cout << p << endl;
        restoreDir (p, backupFile);
    }
}

} // namespace randomly
