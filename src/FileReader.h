#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <string>

class FileReader : public std::ifstream
{
public:
    FileReader()
        : std::ifstream(), m_filePath() {}

    FileReader(const std::string &t_fpath)
        : std::ifstream(t_fpath, std::ios::in), m_filePath(t_fpath) {}

    ~FileReader()
    {
        if (is_open())
            close();
    }

    // \returns Path of file
    std::string filePath() const
    {
        return m_filePath;
    }

private:
    std::string m_filePath;
};

#endif // FILE_READER_H
