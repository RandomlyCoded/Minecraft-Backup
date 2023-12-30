#include <iostream>
#include <filesystem>

#include "process.h"
#include "utils.h"

using namespace std;
using namespace randomly;

void backupDir (const string &world, const filesystem::path &dir, const filesystem::path &backupFile)
{
    cout << "backup "
         << colorCode (Blue) << styleCode (Bold)
         << (dir.empty () ? "world root" : dir.relative_path ().c_str ())
         << colorCode (Default) << endl;

    // add directory to tar file
    call ({"tar",  "--exclude=" + (world / dir / "*" / "*").string(),
                   "-rf", backupFile, world / dir});

    /**
     * when not executing the command via shell (as system() would do), dir/ * would not get expanded, causing tar to try to add "dir/ *" instead of dir/files
     * that means we can't use the "--no-recurion" - option
     * but we can filter out directories manually by excluding dir/ * / *
    **/
}

void backupSave (string world)
{
    cout << "starting to backup "
         << colorCode (Magenta) << styleCode (Bold)
         << world
         << colorCode (Default) << endl;

    // we need to replace every whitespace with "\ "
    string worldFixed;

    for (auto c: world) {
        if (c == ' ')
            worldFixed += '\\';
        worldFixed += c;
    }

    const auto backupFile = filesystem::path("..") / "backups" / (worldFixed + "-backup.tar.xz");

    // clear previous backup
    cout << "clearing previous backup" << endl;

    std::filesystem::remove(backupFile);

    // backup files in the world root
    backupDir (worldFixed, "", backupFile);

    // backup all the directories from the main world
    for (auto& p : filesystem::recursive_directory_iterator (world))
        if (p.is_directory ()) {
            auto path = filesystem::path (p.path().c_str () + world.length () + 1); // remove "world/" from beginning
            backupDir (worldFixed, path, backupFile);
        }

    cout << colorCode (Red) << styleCode (Bold)
         << "backup done! "
         << colorCode (Default)
         << "backup file: "
         << colorCode (Blue) << styleCode (Bold) << backupFile
         << colorCode (Default) << endl;
}

int main ()
{
    string world;

    cout << colorCode (Red) << styleCode (Bold)
         << "enter save directory name"
         << colorCode (Default) << endl;

    getline (cin, world); // getline reads a whole line with whitespaces

    if (!filesystem::exists(world)) {
        cout << colorCode (Red) << styleCode (Bold) << styleCode (Underline)
             << "world does not exist, here are all available ones: "
             << colorCode (Default) << endl;

        for (auto& p : filesystem::directory_iterator (world))
            if (p.is_directory ())
                cout << p << endl;

        return 1;
    }

    backupSave (world);

    return 0;
}
