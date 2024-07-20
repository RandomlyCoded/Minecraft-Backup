#ifndef RANDOMLY_BACKUP_H
#define RANDOMLY_BACKUP_H

#include <archive.h>
#include <filesystem>

#include "commandLine.h"

namespace randomly
{

class Backup
{
public:
    Backup(Options options) : m_options(options)
        {};

    void backupSave(std::string world);
    void restoreSave(std::string world);

    void updateUiState(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile);

private:
    void backupDir(const std::filesystem::path &dir, archive *a);
    void restoreDir(const std::filesystem::path &dir, const std::filesystem::path &backupFile, const char *prefix);

private:
    Options m_options;

    std::filesystem::path m_currentlyInProgress;

    void setTerminalProgressBar(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile);
    void setGUIProgressState(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile);
};

} // namespace randomly

#endif // RANDOMLY_BACKUP_H
