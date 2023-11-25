#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <memory>
#include <vector>
#include "Token.h"
#include "FileReader.h"

class Tokenizer
{
public:
    Tokenizer() noexcept
        : m_fileReader(nullptr) {}

    Tokenizer(const FileReader &t_fileReader) noexcept
        : m_fileReader(t_fileReader) {}

    virtual std::vector<std::unique_ptr<Token>> tokenize() = 0;

protected:
    FileReader m_fileReader;
};

#endif // TOKENIZER_H