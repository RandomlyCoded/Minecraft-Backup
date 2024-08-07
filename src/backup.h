#ifndef RANDOMLY_BACKUP_H
#define RANDOMLY_BACKUP_H

#include <archive.h>
#include <filesystem>

#include "commandLine.h"

namespace randomly
{

class Backup;

// base class to abstract away the GUI, create a derived class and pass an instance when creating the backup to handle your own GUI
class GUIManager
{
public:
    GUIManager(Options &opt) : m_options(opt) {}
    virtual void update(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile, const std::filesystem::path &currentDirectory) = 0;


protected:
    Backup *m_backup;
    Options &m_options;

private:
    friend Backup;
    void setBackup(Backup *backup) { m_backup = backup; } // only called when constructing a Backup
};

class Backup
{
public:
    Backup(Options &options, GUIManager *guiMgr = nullptr);

    void backup() { backupSave(m_options.world); }
    void restore() { restoreSave(m_options.world); }

    void backupSave(std::string world);
    void restoreSave(std::string world);

    void updateUiState(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile);

private:
    void backupDir(const std::filesystem::path &dir, archive *a);
    void restoreDir(const std::filesystem::path &dir, const std::filesystem::path &backupFile, const char *prefix);

private:
    Options &m_options;
    GUIManager *m_guiMgr;

    std::filesystem::path m_currentlyInProgress;

    void setTerminalProgressBar(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile);
    void setGUIProgressState(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile);
};

} // namespace randomly

#endif // RANDOMLY_BACKUP_H
