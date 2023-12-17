#ifndef TOKEN_PARSER_H
#define TOKEN_PARSER_H

#include <vector>
#include <memory>
#include <exception>
#include <unordered_set>
#include <unordered_map>
#include "Token.h"

class TokenParser
{
public:
    TokenParser() noexcept
        : m_readPtr(0), m_tokens(), m_counters() {}

    TokenParser(const std::vector<std::shared_ptr<Token>> &t_tokens, const size_t &t_readPtr = 0) noexcept
        : m_readPtr(t_readPtr), m_tokens(t_tokens), m_counters() {}

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
        incCounters(nextToken);
        incPtr();

        return nextToken;
    }

    // \brief Add enum to be counted
    // \param t_counter Enum to be counted
    void addCounter(const size_t &t_counter) noexcept
    {
        m_counters.insert(std::make_pair(t_counter, 0));
    }

    // \brief Get counted occurrences of enum
    // \param t_counter Counter
    // \returns Occurrences of enum
    size_t counter(const size_t &t_counter) const
    {
        if (m_counters.count(t_counter))
            return m_counters.at(t_counter);
        else
            throw std::out_of_range("Counter does not exist.");
    }

    // \brief Set new tokens to be parsed
    // \param t_tokens New tokens
    // \note Does not reset counters
    void setTokens(const std::vector<std::shared_ptr<Token>> &t_tokens)
    {
        m_tokens = t_tokens;

        // Reset read pointer
        m_readPtr = 0;
    }

    // \brief Clear tokens from RAM
    void clearTokens()
    {
        setTokens(std::vector<std::shared_ptr<Token>>());
    }

    // \brief Get size of total tokens currently being parsed
    // \returns Size of total tokens
    size_t size() const noexcept
    {
        return m_tokens.size();
    }

private:
    // \brief Check if `m_readPtr` is in-range
    // \throws std::range_error if `m_readPtr` is out-of-range
    void throwIfOutOfRange() const
    {
        if (m_readPtr >= m_tokens.size())
            throw std::range_error("Read pointer >= " + std::to_string(m_tokens.size()) + ".");
    }

    // \brief Check if `t_token` should be counted, if, increment it's counters value
    inline void incCounters(const std::shared_ptr<Token> &t_token) noexcept
    {
        if (m_counters.count(t_token->type()))
            m_counters.at(t_token->type())++;
    }

    size_t m_readPtr;
    std::vector<std::shared_ptr<Token>> m_tokens;
    std::unordered_map<size_t, size_t> m_counters;
};

#endif // TOKEN_PARSER_H
