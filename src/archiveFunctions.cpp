#include "archiveFunctions.h"

#include "backup.h"

#include <archive_entry.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

namespace randomly
{

namespace
{

inline int countFiles(filesystem::path dir)
{
    int amt = 0;

    for (const auto &e: filesystem::directory_iterator(dir))
        if(e.is_regular_file())
            ++amt;

    return amt;
}

} // namespace

archive *openWriteArchive(filesystem::path archive)
{
    auto a = archive_write_new();
    archive_write_add_filter_gzip(a);
    archive_write_set_format_pax_restricted(a);
    archive_write_open_filename(a, archive.string().c_str());

    return a;
}

void closeWriteArchive(archive *a)
{
    archive_write_close(a);
    archive_write_free(a);
}

void addEntryToArchive(archive *a, Backup *backup, int fileAmt, int &fileIndex, const filesystem::directory_entry &_entry)
{
    struct archive_entry *entry;
    struct stat st;
    char buff[8192];
    int len;
    int fd;

    backup->updateUiState(fileAmt, fileIndex, _entry.path());
    ++fileIndex;

    stat(_entry.path().c_str(), &st);

    entry = archive_entry_new();

    if (_entry.is_directory())
        archive_entry_set_filetype(entry, AE_IFDIR);
    else
        archive_entry_set_filetype(entry, AE_IFREG);

    archive_entry_set_pathname(entry, _entry.path().c_str());

    const auto lastModified = chrono::file_clock::to_sys(_entry.last_write_time());

    archive_entry_set_mtime(entry, chrono::system_clock::to_time_t(lastModified), 0);
    archive_entry_set_size(entry, st.st_size);
    archive_entry_set_perm(entry, 0644);

    archive_write_header(a, entry);

    fd = open(_entry.path().c_str(), O_RDONLY);
    len = read(fd, buff, sizeof(buff));

    while ( len > 0 ) {
        archive_write_data(a, buff, len);
        len = read(fd, buff, sizeof(buff));
    }

    close(fd);
    archive_entry_free(entry);
}

void writeToArchive(archive *a, filesystem::path dir, Backup *backup)
{

    const int fileAmt = countFiles(dir);
    int fileIndex = -1; // start at -1 since adding the current directory would lead to counting too far and breaking the display

    // we need to add the current directory
    addEntryToArchive(a, backup, fileAmt, fileIndex, filesystem::directory_entry(dir));

    for (const auto &file: filesystem::directory_iterator (dir)) {
        if (file.is_directory())
            continue; //skip it since directories are saved separately

        addEntryToArchive(a, backup, fileAmt, fileIndex, file);
    }
}

int copy_data(archive *ar, archive *aw)
{
    int r;
    const void *buff;
    size_t size;
#if ARCHIVE_VERSION >= 3000000
    int64_t offset;
#else
    off_t offset;
#endif

    while(true) {
        r = archive_read_data_block(ar, &buff, &size, &offset);

        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r != ARCHIVE_OK)
            return (r);

        r = archive_write_data_block(aw, buff, size, offset);

        if (r != ARCHIVE_OK) {
            cerr << "archive_write_data_block(): " << archive_error_string(aw) << endl;
            return (r);
        }
    }
}

void readFromArchive(filesystem::path archive, filesystem::path dir, const char *prefix, Backup *p)
{
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int r;
    int flags = ARCHIVE_EXTRACT_TIME;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);


    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);

    archive_read_support_format_tar(a);

    if (archive_read_open_filename(a, archive.c_str(), 10240)) {
        cerr  << "archive_read_open_file() failed: " << archive_error_string(a) << endl;
        return;
    }

    while (true) {
        r = archive_read_next_header(a, &entry);

        if (r == ARCHIVE_EOF)
            break;
        if (r != ARCHIVE_OK)
            cerr << "archive_read_next_header() failed: " << archive_error_string(a) << endl;

        auto p = filesystem::path (archive_entry_pathname(entry));

        // we need to prepend the prefix
        const auto actualPath = prefix + p.string();
        archive_entry_set_pathname(entry, actualPath.c_str());

        // reassign since we changed it
        p = filesystem::path (archive_entry_pathname(entry));

        // path / "" since we use a trailing directory separator to differenciate between directories and files
        if ((p.parent_path() / "") != dir)
            continue;

        r = archive_write_header(ext, entry);

        if (r != ARCHIVE_OK)
            cerr << "archive_write_header(): " << archive_error_string(ext) << endl;

        else {
            copy_data(a, ext);
            r = archive_write_finish_entry(ext);

            if (r != ARCHIVE_OK) {
                cerr << "archive_write_finish_entry()" << archive_error_string(ext) << endl;
                break;
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);
}

vector<filesystem::path> readDirectories(filesystem::path archive)
{
    vector<filesystem::path> directories;

    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    r = archive_read_open_filename(a, archive.c_str(), 10240);
    if (r != ARCHIVE_OK)
        exit(1);

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        if(!filesystem::path(archive_entry_pathname(entry)).has_filename()) // no filename means directory
            directories.push_back(archive_entry_pathname(entry));

        archive_read_data_skip(a);
    }

    r = archive_read_free(a);

    if (r != ARCHIVE_OK)
        exit(1);

    return directories;
}

} // namespace randomly
