#include <iostream>
#include <filesystem>
#include <vector>

#include "process.h"
#include "utils.h"

using namespace std;
using namespace randomly;

void restoreDir (const filesystem::path &dir, const filesystem::path &backupFile)
{
    cout << "restoring "
         << colorCode (Blue) << styleCode (Bold)
         << (dir.empty () ? "world root" : dir.relative_path ().c_str ())
         << colorCode (Default) << endl;

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

int main (int argc, char **argv)
{
    parseCommandLine(argc, argv);

    if (options.world.empty()) {
        cout << colorCode (Red) << styleCode (Bold)
             << "enter save directory name"
             << colorCode (Default) << endl;

        getline (cin, options.world); // getline reads a whole line with whitespaces
    }
    else
        cout << "using world from command line" << endl;

    if (!filesystem::exists(options.savesDirectory / options.world)) {
        cout << colorCode (Red) << styleCode (Bold) << styleCode (Underline)
             << "world does not exist, here are all available ones: "
             << colorCode (Default) << endl;

        for (auto& p : filesystem::directory_iterator (options.savesDirectory))
            if (p.is_directory ())
                cout << p << endl;

        return 1;
    }

    filesystem::current_path(options.savesDirectory); // set the working path

    restoreSave (options.world);

    return 0;
}
