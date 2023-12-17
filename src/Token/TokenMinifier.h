#ifndef TOKEN_MINIFIER_H
#define TOKEN_MINIFIER_H

#include <vector>
#include <memory>
#include "Token.h"
#include "TokenParser.h"

class TokenMinifier
{
public:
    TokenMinifier() noexcept
        : m_tp() {}
    
    TokenMinifier(const TokenParser &t_tp) noexcept
        : m_tp(t_tp) {}

    TokenParser tp() const noexcept
    {
        return m_tp;
    }

    // \brief Minify tokens by reference
    // \param t_tokens Tokens to be minified
    virtual void minify(std::vector<std::shared_ptr<Token>> &t_tokens) = 0;

protected:
    TokenParser m_tp;
};

#endif // TOKEN_MINIFIER_H
