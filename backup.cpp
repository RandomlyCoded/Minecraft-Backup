#include <iostream>
#include <filesystem>

using namespace std;

void backupDir (string world, filesystem::path dir)
{
    cout << "backing up \033[01;34m" << (dir.empty () ? "world root" : dir.relative_path ().c_str ()) << "\033[00m" << endl;

    // create directory
    system (("mkdir " + world + "-BACKUP/" + dir.relative_path ().string () + "/ 2> /dev/null").c_str ());

    // copy content
    system (("cp " + world + "/" + dir.relative_path ().string () + "/* " + world + "-BACKUP/" + dir.relative_path ().string () + "/ 2> /dev/null").c_str ());
}

void backupSave (string world)
{
    cout << "starting to back up \033[01;35m" << world << "\033[00m" << endl;

    // we need to replace every whitespace with "\ " for cp to work properly
    string worldFixed;

    for (auto c: world) {
        if (c == ' ')
            worldFixed += '\\';
        worldFixed += c;
    }

    world = worldFixed;

    // clear previous backup
    system (("rm " + world + "-BACKUP/ -r 2> /dev/null").c_str ());

    // create backup
    system (("mkdir " + world + "-BACKUP/").c_str ());

    // backup files in the world root
    backupDir (world, "");

    // backup all the directories from the main world
    for (auto& p : filesystem::recursive_directory_iterator (world))
        if (p.is_directory ()) {
            auto path = filesystem::path (p.path().c_str () + world.length () + 1); // remove "world/" from beginning
            backupDir (world, path);
        }
}

int main ()
{
    string world;

    cout << "\033[01;31menter save directory name\033[00m: " << endl;
    getline (cin, world); // getline reads a whole line with whitespaces

    if (!filesystem::exists(world)) {
        cout << "\033[01;04;31mworld does not exist, here are all available ones\033[00m" << endl;
        system ("ls");

        return 1;
    }

    backupSave (world);

    return 0;
}
