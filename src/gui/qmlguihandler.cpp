#include "qmlguihandler.h"

#include <QDebug>

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

void QmlGuiHandler::reset()
{
    m_filesProcessed = 1;
    m_filesTotal = 1;

    m_currentFile = "";
    m_currentDirectory = "";

    emit ping();
}

} // namespace randomly
