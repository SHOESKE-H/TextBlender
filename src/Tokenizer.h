#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <memory>
#include <vector>
#include <string>
#include "Token.h"
#include "FileReader.h"

class Tokenizer
{
public:
    Tokenizer() noexcept
        : m_fileReader(nullptr) {}

    Tokenizer(const std::string &t_fpath) noexcept
        : m_fileReader(t_fpath) {}

    virtual std::vector<std::unique_ptr<Token>> tokenize() = 0;

protected:
    FileReader m_fileReader;
};

#endif // TOKENIZER_H
