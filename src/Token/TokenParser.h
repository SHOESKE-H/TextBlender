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
        : m_readPtr(0), m_failed(0), m_detectFail(0), m_tokens() {}

    TokenParser(const std::vector<std::shared_ptr<Token>> &t_tokens, const size_t &t_readPtr = 0, const size_t &t_detectFail = 0) noexcept
        : m_readPtr(t_readPtr), m_failed(0), m_detectFail(t_detectFail), m_tokens(t_tokens) {}

    // \brief Returns failed number of tokens
    // \brief Failed number of tokens parsed so far
    size_t failed() const noexcept
    {
        return m_failed;
    }

    // \brief Increment read pointer, return value after increment
    // \returns Read pointer after increment
    size_t incPtr() noexcept
    {
        return ++m_readPtr;
    }

    // \brief Check whether the read pointer has reached the end
    // \brief True if the read pointer has reached the end, otherwise false
    bool end() const noexcept
    {
        return m_readPtr >= m_tokens.size();
    }

    // \brief Increment read pointer, return value before increment
    // \returns Read pointer before increment
    size_t ptrInc() noexcept
    {
        return m_readPtr++;
    }

    // \brief Get position of read pointer
    size_t ptr() const noexcept
    {
        return m_readPtr;
    }

    // \brief Skip tokens
    // \param t_n Number of tokens to skip
    // \returns Read pointer after applying the skip
    size_t skip(const size_t &t_n) noexcept
    {
        return m_readPtr += t_n;
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
        incFail(nextToken);
        incPtr();
        return nextToken;
    }

private:
    // \brief Check if `m_readPtr` is in-range
    // \throws std::range_error if `m_readPtr` is out-of-range
    void throwIfOutOfRange() const
    {
        if (m_readPtr >= m_tokens.size())
            throw std::range_error("Read pointer >= " + std::to_string(m_tokens.size()) + ".");
    }

    // \brief Check if `t_token` is equal to the set failed enum, and if it is, increment the failed counter
    inline void incFail(const std::shared_ptr<Token> &t_token) noexcept
    {
        if (t_token->type() == m_detectFail)
            m_failed++;
    }

    size_t m_readPtr;
    size_t m_failed;
    size_t m_detectFail;
    std::vector<std::shared_ptr<Token>> m_tokens;
};

#endif // TOKEN_PARSER_H
