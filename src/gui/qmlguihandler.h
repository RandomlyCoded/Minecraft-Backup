#ifndef QMLGUIHANDLER_H
#define QMLGUIHANDLER_H

#include "backup.h"

#include <QObject>
#include <QThread>

namespace randomly
{

class BackupThread : public QThread
{
public:
    BackupThread(QObject *parent = nullptr)
        : QThread(parent)
    {}

    // I think you can do it in a better way, but I'll stick to the intended use by calling start()

    // also we can't pass the backup when initializing, since the Backup is created after the GUI manager
    void backup(Backup *backup) {
        if (isRunning())
            return;

        m_backup = backup;
        m_mode = Backup;
        start();
    }
    void restore (Backup *backup) {
        if (isRunning())
            return;

        m_backup = backup;
        m_mode = Restore;
        start();
    }

protected:
    void run()
    {
        switch(m_mode) {
        case Restore: m_backup->restore(); break;
        case Backup: m_backup->backup(); break;
        }
    };

private:
    Backup *m_backup;

    enum Mode {
        Restore, Backup
    } m_mode;
};

class QmlGuiHandler : public QObject, public GUIManager
{
    Q_OBJECT

    Q_PROPERTY(QString currentDirectory READ currentDirectory NOTIFY ping)
    Q_PROPERTY(QString currentFile READ currentFile NOTIFY ping)
    Q_PROPERTY(int filesProcessed READ filesProcessed NOTIFY ping)
    Q_PROPERTY(int filesTotal READ filesTotal NOTIFY ping)

    Q_PROPERTY(QString world READ world WRITE setWorld NOTIFY dummySignal) // add dummy signal so QML thinks it is notifyable and doesn't produce errors
    Q_PROPERTY(QString directory READ directory WRITE setDirectory NOTIFY dummySignal)

    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    // GUIManager interface
    QmlGuiHandler(Options &opt, QObject *parent = nullptr);
    void update(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile, const std::filesystem::path &currentDirectory);

    // Q_PROPERTY getters
    QString currentFile() const;
    QString currentDirectory() const;

    int filesProcessed() const;
    int filesTotal() const;

    QString world() const;
    QString directory() const;

    bool running() {
        return m_worker->isRunning();
    }

public slots:
    void startBackup();
    void startRestore();

    void setWorld(QString world);
    void setDirectory(QString dir);

    void openBackupDirectory();
    void openSavesDirectory();

    bool doesDirectoryExist(QString dir);

signals:
    void ping();

    void dummySignal();

    void runningChanged();

private:
    std::filesystem::path m_currentFile;
    std::filesystem::path m_currentDirectory;
    int m_filesProcessed = 1;
    int m_filesTotal = 1;

    void reset
        ();;

    BackupThread *m_worker;
   QString m_directory;
};

} // namespace randomly

#endif // QMLGUIHANDLER_H
