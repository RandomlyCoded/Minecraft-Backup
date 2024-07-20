#ifndef QMLGUIHANDLER_H
#define QMLGUIHANDLER_H

#include "backup.h"
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

public:
    // GUIManager interface
    QmlGuiHandler(Options &opt) : GUIManager(opt) {}
    void update(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile, const std::filesystem::path &currentDirectory);

    void setParent(QObject *parent) { QObject::setParent(parent); } // need to overload this to set the parent after initialization

    // Q_PROPERTY getters
    std::filesystem::path currentFile() const;
    std::filesystem::path currentDirectory() const;

    int filesProcessed() const;
    int filesTotal() const;

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
