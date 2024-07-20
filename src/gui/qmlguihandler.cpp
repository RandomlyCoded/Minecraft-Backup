#include "qmlguihandler.h"

#include <QDebug>

namespace randomly
{

void QmlGuiHandler::update(int filesTotal, int filesProcessed, const std::filesystem::__cxx11::path &currentFile, const std::filesystem::__cxx11::path &currentDirectory)
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

} // namespace randomly
