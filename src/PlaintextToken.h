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
    } TokenType;

    PlaintextToken() noexcept
        : Token() {}

    PlaintextToken(const TokenType &t_type, const std::string &t_value) noexcept
        : Token(t_type, t_value) {}
};