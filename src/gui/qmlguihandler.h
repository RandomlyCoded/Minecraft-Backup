#ifndef QMLGUIHANDLER_H
#define QMLGUIHANDLER_H

#include "backup.h"
#include <QDir>
#include <QObject>

namespace randomly
{

class QmlGuiHandler : public QObject, public GUIManager
{
    Q_OBJECT

    Q_PROPERTY(std::filesystem::path currentDirectory READ currentDirectory NOTIFY ping)
    Q_PROPERTY(std::filesystem::path currentFile READ currentFile NOTIFY ping)
    Q_PROPERTY(int filesProcessed READ filesProcessed NOTIFY ping)
    Q_PROPERTY(int filesTotal READ filesTotal NOTIFY ping)

    Q_PROPERTY(QString world READ world WRITE setWorld FINAL)
    Q_PROPERTY(QDir directory READ directory WRITE setDirectory FINAL)

public:
    // GUIManager interface
    QmlGuiHandler(Options &opt, QObject *parent = nullptr) : QObject(parent), GUIManager(opt) {}
    void update(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile, const std::filesystem::path &currentDirectory);

    // Q_PROPERTY getters
    std::filesystem::path currentFile() const;
    std::filesystem::path currentDirectory() const;

    int filesProcessed() const;
    int filesTotal() const;

    QString world() const;
    QDir directory() const;

public slots:
    void startBackup();
    void startRestore();

    void setWorld(QString world);
    void setDirectory(QDir dir);

signals:
    void ping();

private:
    std::filesystem::path m_currentFile;
    std::filesystem::path m_currentDirectory;
    int m_filesProcessed;
    int m_filesTotal;
};

} // namespace randomly

#endif // QMLGUIHANDLER_H
