#include <iostream>
#include <filesystem>

#include "process.h"
#include "fancyColoring.h"

using namespace std;
using namespace randomly;

void restoreDir (string world, filesystem::path dir)
{
    cout << "restoring "
         << colorCode (Blue) << styleCode (Bold)
         << (dir.empty () ? "world root" : dir.relative_path ().c_str ())
         << colorCode (Default) << endl;

    // create directory
    randomly::call ({("mkdir " + world + "/" + dir.relative_path ().string () + "/ 2> /dev/null")});

    // copy content
    randomly::call ({("cp " + world + "-BACKUP/" + dir.relative_path ().string () + "/* " + world + "/" + dir.relative_path ().string () + "/ 2> /dev/null")});
}

void restoreSave (string world)
{
    cout << "restoring world "
         << colorCode (Magenta) << styleCode (Bold)
         << world
         << colorCode (Default) << endl;

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

    restoreSave (world);

    return 0;
}
