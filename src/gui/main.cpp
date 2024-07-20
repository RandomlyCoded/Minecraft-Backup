#include "backup.h"
#include "commandLine.h"
#include "utils.h"

#include "qmlguihandler.h"

using namespace std;

using namespace randomly;

// Helper for underlining the first letter of a text
struct FirstLetterUnderlined
{
    const char *txt;

private:
    friend std::ostream& operator<<(std::ostream& os, FirstLetterUnderlined text)
    {
        os << (Green | Bold | Underline) << text.txt[0];

        os << (Green | Bold | UnderlineOff) << (text.txt + 1);

        return os;
    }
};


int main(int argc, char **argv)
{
    auto options = parseCommandLine(argc, argv);

    if (!options.backupSet) {
        cout << (Green | Bold)
             << FirstLetterUnderlined{"Backup or "} << FirstLetterUnderlined{"Restore world? (B / r)"} << endl
             << Default;

        string response;
        getline(cin, response);

        if (response.empty() || response[0] == 'B' || response[0] == 'b')
            options.backup = true;

        else if(response[0] == 'r' || response[0] == 'R')
            options.backup = false;

        else {
            cerr << (Red | Underline)
                 << "invalid response." << endl
                 << Default;

            return EXIT_FAILURE;
        }
    }

    if (options.world.empty()) {
        cout << (Green | Bold)
             << "enter save name" << endl
             << Default;

        getline (cin, options.world); // getline reads a whole line with whitespaces
    }
    else
        cout << "using world from command line" << endl;

    options.useGUI = true;

    Backup backup(options, new QmlGuiHandler{options});

    if(options.backup)
        backup.backupSave(options.world);

    else
        backup.restoreSave(options.world);

    return 0;
}
