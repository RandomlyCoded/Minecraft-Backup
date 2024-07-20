#include "qmlguihandler.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

namespace randomly
{

QmlGuiHandler::QmlGuiHandler(Options &opt, QObject *parent)
    : QObject(parent),
    GUIManager(opt),
    m_worker(new BackupThread{this})
{
    connect(m_worker, &QThread::started, this, &QmlGuiHandler::runningChanged);

    connect(m_worker, &QThread::finished, this, [this] {
        reset();
        emit runningChanged();
    });
}

void QmlGuiHandler::update(int filesTotal, int filesProcessed, const std::filesystem::path &currentFile, const std::filesystem::path &currentDirectory)
{
    m_filesTotal = filesTotal;
    m_filesProcessed = filesProcessed;

    m_currentFile = currentFile;
    m_currentDirectory = currentDirectory;

    emit ping();
}

QString QmlGuiHandler::currentFile() const
{ return QString::fromStdString(m_currentFile.string()); }

QString QmlGuiHandler::currentDirectory() const
{ return QString::fromStdString(m_currentDirectory.string()); }

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
    m_worker->backup(m_backup);
}

void QmlGuiHandler::startRestore()
{
    m_worker->restore(m_backup);
}

void QmlGuiHandler::setWorld(QString world)
{
    m_options.world = world.toStdString();
}

void QmlGuiHandler::setDirectory(QDir dir)
{
    m_options.savesDirectory = dir.path().toStdString();
}

void QmlGuiHandler::openBackupDirectory()
{
    auto path = m_options.savesDirectory / ".." / "backups";
    auto url = QUrl{path.c_str()};
    QDesktopServices::openUrl(url);
}

void QmlGuiHandler::openSavesDirectory()
{
    // apparently, QDesktopServices will fail to launch a file explorer if the requested directory is the current directory
    // at least on Debian

    // BUT using an URL like this works (the directory "none" doesn't actually need to exist)

    auto path = m_options.savesDirectory / "none" / "..";
    auto url = QUrl{path.c_str()};
    QDesktopServices::openUrl(url);
}

void QmlGuiHandler::reset()
{
    m_filesProcessed = 1;
    m_filesTotal = 1;

    m_currentFile = "";
    m_currentDirectory = "";

    emit ping();
}

} // namespace randomly
