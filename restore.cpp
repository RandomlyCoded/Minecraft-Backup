#include <iostream>
#include <filesystem>

using namespace std;

void sysWrap(const char *cmd)
{
#ifdef DEBUG_BUILD
    cout << "\033[01;33m" << cmd << "\033[0m\b" << endl;
#endif // DEBUG_BUILD

    system(cmd);
}

void restoreDir (string world, filesystem::path dir)
{
    cout << "restoring \033[01;34m" << (dir.empty () ? "world root" : dir.relative_path ().c_str ()) << "\033[00m" << endl;

    // create directory
    sysWrap (("mkdir " + world + "/" + dir.relative_path ().string () + "/ 2> /dev/null").c_str ());

    // copy content
    sysWrap (("cp " + world + "-BACKUP/" + dir.relative_path ().string () + "/* " + world + "/" + dir.relative_path ().string () + "/ 2> /dev/null").c_str ());
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
    sysWrap (("rm " + world + "/ -r").c_str ());

    // create save directory
    sysWrap (("mkdir " + world + "/").c_str ());

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
        sysWrap ("ls");

        return 1;
    }

    restoreSave (world);

    return 0;
}
