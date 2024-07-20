#ifndef RANDOMLY_ARCHIVEFUNCTIONS_H
#define RANDOMLY_ARCHIVEFUNCTIONS_H

#include <archive.h>
#include <filesystem>
#include <vector>

namespace randomly
{

class Backup;

archive *openWriteArchive(std::filesystem::path archive);
void closeWriteArchive(archive *a);

void writeToArchive (archive *a, std::filesystem::path dir, Backup *backup);

int copy_data(struct archive *ar, struct archive *aw);

void readFromArchive (std::filesystem::path archive, std::filesystem::path dir, const char *prefix, Backup *backup);
std::vector<std::filesystem::path> readDirectories (std::filesystem::path archive);

} // namespace randomly

#endif // RANDOMLY_ARCHIVEFUNCTIONS_H
