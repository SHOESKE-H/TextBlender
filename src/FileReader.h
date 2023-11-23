#ifndef FILEREADER_H
#define FILEREADER_H

#include <exception>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class EofException : public std::exception
{
public:
    EofException(const std::string &t_msg)
        : m_msg(t_msg) {}

    const char *what() const noexcept override
    {
        return m_msg.c_str();
    }

private:
    std::string m_msg;
};

class FileReader
{
public:
    FileReader()
        : m_filePath(), m_file(nullptr) {}

    FileReader(const std::string &t_fpath)
        : m_filePath(t_fpath), m_file(new std::ifstream(t_fpath, std::ios::in))
    {
        throwIfBadFile();
    }

    ~FileReader()
    {
        m_file->close();
        delete m_file;
    }

    // \returns Path of currently read file
    std::string filePath() const
    {
        return m_filePath;
    }

    typedef enum
    {
        Null,
        Fail,
        Bad,
        EndOfFile,
        Good,
    } FileState;

    // \brief Check the file state
    // \returns Current state of file as a FileState enum
    FileState fileState() const
    {
        if (!m_file)
        {
            return FileState::Null;
        }
        else if (m_file->eof())
        {
            return FileState::EndOfFile;
        }
        else if (m_file->fail())
        {
            return FileState::Fail;
        }
        else if (m_file->bad())
        {
            return FileState::Bad;
        }
        else
        {
            return FileState::Good;
        }
    }

    // \brief Read the next line of file
    // \returns Next line of file
    std::string readLine()
    {
        throwIfBadFile();
        throwIfEof();

        std::string line;
        getline(*m_file, line);
        return line;
    }

    // \brief Read a specified length of characters
    // \param t_n The number of characters to be read
    // \throws `std::invalid_argument` if `t_n` is less than 1
    std::string read(const size_t &t_n)
    {
        if (t_n < 1)
            throw std::invalid_argument("Cannot read less than 1 character.");

        throwIfBadFile();
        throwIfEof();

        std::string buffer(t_n, '\0');
        m_file->read(buffer.data(), t_n);
        return buffer;
    }

    // \brief Move the read pointer to a specified location in the file
    // \param off The offset position relative to the direction specified by 'dir'
    // \param dir The point of reference for the offset. It can be the beginning, current position, or end of the file. Default is the beginning of the file.
    void seek(const std::streamoff &off, const std::ios_base::seekdir &dir = std::ios::beg)
    {
        throwIfBadFile();
        m_file->seekg(off, dir);
    }

    // \brief Clear failbit
    void clear()
    {
        m_file->clear();
    }

    // \brief Returns a pointer to the file buffer that is associated with the input file stream.
    // \details This function is used to get direct, low-level access to the input file buffer,
    // which may be useful for advanced file operations.
    // \returns A pointer to the file buffer (std::filebuf*). If the file has not been opened,
    // this will point to a buffer that is not associated with any file.
    std::filebuf* readBuffer()
    {
        return m_file->rdbuf();
    }

private:
    // \brief Check if the file is operable on
    // \throws `std::runtime_error` if `m_file` is bad
    void throwIfBadFile()
    {
        switch (fileState())
        {
        case FileState::Good:
            break;

        case FileState::Fail:
            break;

        case FileState::EndOfFile:
            break;

        case FileState::Bad:
            if (!m_file->is_open())
                throw std::runtime_error("File not open.");

            throw std::runtime_error("Bad file.");
            break;

        case FileState::Null:
            throw std::runtime_error("File is null.");
            break;

        default:
            throw std::runtime_error("File in unknown state.");
            break;
        }
    }

    // \brief Check if the eof bit is set
    // \throws `EofException` if the eof bit of `m_file` is set
    void throwIfEof()
    {
        if (m_file->eof())
            throw EofException("Reached the end of file.");
    }

    std::string m_filePath;
    std::ifstream *m_file;
};

#endif // FILEREADER_H
