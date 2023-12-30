#include <iostream>
#include <filesystem>

#include "process.h"

using namespace std;

void restoreDir (string world, filesystem::path dir)
{
    cout << "restoring \033[01;34m" << (dir.empty () ? "world root" : dir.relative_path ().c_str ()) << "\033[00m" << endl;

    // create directory
    randomly::call ({("mkdir " + world + "/" + dir.relative_path ().string () + "/ 2> /dev/null")});

    // copy content
    randomly::call ({("cp " + world + "-BACKUP/" + dir.relative_path ().string () + "/* " + world + "/" + dir.relative_path ().string () + "/ 2> /dev/null")});
}

void restoreSave (string world)
{
    cout << "restoring \033[01;35m" << world << "\033[00m" << endl;

    // we need to replace every whitespace with "\ " for cp to work properly
    string worldFixed;

    for (auto c: world) {
        if (c == ' ')
            worldFixed += '\\';
        worldFixed += c;
    }

    world = worldFixed;

    // clear save
    randomly::call ({("rm " + world + "/ -r")});

    // create save directory
    randomly::call ({("mkdir " + world + "/")});

    // restore files in the world root
    restoreDir (world, "");

    // restore all the directories from the backup world
    for (auto& p : filesystem::recursive_directory_iterator (world + "-BACKUP"))
        if (p.is_directory ()) {
            auto path = filesystem::path (p.path().c_str () + world.length () + 8); // remove "world-BACKUP/" from beginning
            restoreDir (world, path);
        }
}

int main ()
{
    string world;

    cout << "\033[01;31menter save directory name\033[00m: " << endl;
    getline (cin, world); // getline reads a whole line with whitespaces

    if (!filesystem::exists(world + "-BACKUP")) {
        cout << "\033[01;04;31mbackup does not exist, here are all available ones\033[00m" << endl;
        randomly::call ({"ls"});

        return 1;
    }

    restoreSave (world);

    return 0;
}
