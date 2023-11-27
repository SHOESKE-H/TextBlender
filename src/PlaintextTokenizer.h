#ifndef PLAINTEXT_TOKENIZER_H
#define PLAINTEXT_TOKENIZER_H

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include "Tokenizer.h"
#include "PlaintextToken.h"

class PlaintextTokenizer : public Tokenizer
{
public:
    PlaintextTokenizer() noexcept
        : Tokenizer() {}

    PlaintextTokenizer(const std::string &t_fpath) noexcept
        : Tokenizer(t_fpath) {}

    // \brief Tokenize file into `PlaintextToken`s
    std::vector<std::unique_ptr<Token>> tokenize() override
    {
        // Check file state
        if (!m_fileReader.is_open())
            throw std::runtime_error("File not open.");
        else if (m_fileReader.bad())
            throw std::runtime_error("Bad file.");

        std::vector<std::unique_ptr<Token>> tokens;

        PlaintextToken curToken;
        while ((curToken = token()).type() != PlaintextToken::Type::EndOfFile)
            tokens.push_back(std::make_unique<Token>(curToken));

        // Add final eof token
        tokens.push_back(std::make_unique<Token>(curToken));

        return tokens;
    }

private:
    // \brief Tokenize next token
    // \returns Next token
    PlaintextToken token()
    {
        PlaintextToken token;

        char curChar = 0;
        m_fileReader.get(curChar);

        if (!curChar)
            return PlaintextToken(PlaintextToken::Type::EndOfFile);

        std::string buffer(1, curChar);

        switch (type(curChar))
        {
        case PlaintextToken::Type::Word: // Is a character (a-z, A-Z, 0-9)

            token.setType(PlaintextToken::Type::Word);
            // Read rest of characters from word
            while (m_fileReader.get(curChar))
            {
                if (isWordSeparator(curChar))
                {
                    m_fileReader.putback(curChar);
                    break;
                }
                buffer += curChar;
            }
            break;

        case PlaintextToken::Type::Whitespace: // Is whitespace (space, \t, \n)
            token.setType(PlaintextToken::Type::Whitespace);
            break;

        case PlaintextToken::Type::Punctuation: // Is punctuation
            token.setType(PlaintextToken::Type::Punctuation);
            break;

        default: // Type unknown
            token.setType(PlaintextToken::Type::Unknown);
            break;
        }

        token.setValue(buffer);
        return token;
    }

    // \brief Check if a character should be treated as a word separator
    static bool isWordSeparator(const char &t_char)
    {
        return !std::isalnum(static_cast<unsigned char>(t_char));
    }

    PlaintextToken::Type type(const char &t_char)
    {
        if (std::isalnum(static_cast<unsigned char>(t_char)))
            return PlaintextToken::Type::Word;

        if (std::isblank(static_cast<unsigned char>(t_char)) || t_char == '\n')
            return PlaintextToken::Type::Whitespace;

        if (std::ispunct(static_cast<unsigned char>(t_char)))
            return PlaintextToken::Type::Punctuation;

        return PlaintextToken::Type::Unknown;
    }
};

#endif // PLAINTEXT_TOKENIZER_H
