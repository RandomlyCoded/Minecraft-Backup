#include "qmlguihandler.h"

#include <QDebug>

namespace randomly
{

void QmlGuiHandler::update(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile, const std::filesystem::path &currentDirectory)
{
    m_filesTotal = filesTotal;
    m_filesProcessed = filesProcessed;

    m_currentFile = currentFile;
    m_currentDirectory = currentDirectory;

    emit ping();
}

std::filesystem::path QmlGuiHandler::currentFile() const
{ return m_currentFile; }

std::filesystem::path QmlGuiHandler::currentDirectory() const
{ return m_currentDirectory; }

int QmlGuiHandler::filesProcessed() const
{ return m_filesProcessed; }

int QmlGuiHandler::filesTotal() const
{ return m_filesTotal; }

QString QmlGuiHandler::world() const
{ return QString::fromStdString(m_options.world); }

QDir QmlGuiHandler::directory() const
{
    QString dirName = QString::fromStdString(m_options.savesDirectory.string());
    return QDir::fromNativeSeparators(dirName);
}

void QmlGuiHandler::startBackup()
{
    m_backup->restoreSave(m_options.world);
}

void QmlGuiHandler::startRestore()
{
    m_backup->backupSave(m_options.world);
}

void QmlGuiHandler::setWorld(QString world)
{
    m_options.world = world.toStdString();
}

void QmlGuiHandler::setDirectory(QDir dir)
{
    m_options.savesDirectory = dir.path().toStdString();
}

} // namespace randomly
