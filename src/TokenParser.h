#ifndef TOKEN_PARSER_H
#define TOKEN_PARSER_H

#include <vector>
#include <memory>
#include <exception>
#include "Token.h"

class TokenParser
{
public:
    TokenParser() noexcept
        : m_readPtr(0), m_tokens() {}

    TokenParser(const std::vector<std::shared_ptr<Token>> &t_tokens, const size_t t_readPtr = 0) noexcept
        : m_readPtr(t_readPtr), m_tokens(t_tokens) {}

    // \brief Increment read pointer, return value after increment
    // \returns Read pointer after increment
    size_t incPtr()
    {
        m_readPtr++;
        throwIfOutOfRange();

        return m_readPtr;
    }

    // \brief Check whether the read pointer has reached the end
    // \brief True if the read pointer has reached the end, otherwise false
    bool end() const
    {
        return m_readPtr >= m_tokens.size();
    }

    // \brief Increment read pointer, return value before increment
    // \returns Read pointer before increment
    size_t ptrInc()
    {
        m_readPtr++;
        throwIfOutOfRange();

        return m_readPtr - 1;
    }

    // \brief Get position of read pointer
    size_t ptr() const noexcept
    {
        return m_readPtr;
    }

    // \brief Skip tokens
    // \param t_n Number of tokens to skip
    // \returns Read pointer after applying the skip
    size_t skip(const size_t &t_n)
    {
        m_readPtr += t_n;
        throwIfOutOfRange();

        return m_readPtr;
    }

    // \brief Get the next token
    std::shared_ptr<Token> token() const
    {
        throwIfOutOfRange();

        return m_tokens.at(m_readPtr);
    }

    // \brief Get the next token, increment read pointer
    std::shared_ptr<Token> parseToken()
    {
        throwIfOutOfRange();

        std::shared_ptr<Token> nextToken = token();
        incPtr();
        return nextToken;
    }

private:
    // \brief Check if `m_readPtr` is in-range
    // \throws std::range_error if `m_readPtr` is out-of-range
    void throwIfOutOfRange() const
    {
        if (m_readPtr + 1 > m_tokens.size())
            throw std::range_error("Read pointer >= " + std::to_string(m_tokens.size()) + ".");
    }

    size_t m_readPtr;
    std::vector<std::shared_ptr<Token>> m_tokens;
};

#endif // TOKEN_PARSER_H
