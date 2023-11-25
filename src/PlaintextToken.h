#ifndef PLAINTEXT_TOKEN_H
#define PLAINTEXT_TOKEN_H

#include <string>
#include "Token.h"

class PlaintextToken : public Token
{
public:
    typedef enum
    {
        Unknown,
        Whitespace,
        EndOfFile,
        Word,
        Punctuation
    } Type;

    PlaintextToken() noexcept
        : Token() {}

    PlaintextToken(const Type &t_type, const std::string &t_value) noexcept
        : Token(t_type, t_value) {}
};

#endif // PLAINTEXT_TOKEN_H
