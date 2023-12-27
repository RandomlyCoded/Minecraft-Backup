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

void backupDir (string world, filesystem::path dir)
{
    cout << "backup \033[01;34m" << (dir.empty () ? "world root" : dir.relative_path ().c_str ()) << "\033[00m" << endl;

    // add directory to tar file
    sysWrap (("tar --no-recursion -" + ((dir.empty () ? string("c") : string("r")) + "f ../backups/" + world) + "-backup.tar.xz " + world + "/" + dir.string () + "/*").c_str ());
}

void backupSave (string world)
{
    cout << "starting to backup \033[01;35m" << world << "\033[00m" << endl;

    // we need to replace every whitespace with "\ "
    string worldFixed;

    for (auto c: world) {
        if (c == ' ')
            worldFixed += '\\';
        worldFixed += c;
    }

    // clear previous backup
    sysWrap (("rm ../backups/" + world + "-backup.tar.xz -r").c_str ());

    // backup files in the world root
    backupDir (worldFixed, "");

    // backup all the directories from the main world
    for (auto& p : filesystem::recursive_directory_iterator (world))
        if (p.is_directory ()) {
            auto path = filesystem::path (p.path().c_str () + world.length () + 1); // remove "world/" from beginning
            backupDir (worldFixed, path);
        }
}

int main ()
{
    string world;

    cout << "\033[01;31menter save directory name\033[00m: " << endl;
    getline (cin, world); // getline reads a whole line with whitespaces

    if (!filesystem::exists(world)) {
        cout << "\033[01;04;31mworld does not exist, here are all available ones\033[00m" << endl;
        sysWrap ("ls");

        return 1;
    }

    backupSave (world);

    return 0;
}
