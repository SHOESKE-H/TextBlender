#ifndef TOKEN_PARSER_H
#define TOKEN_PARSER_H

#include <vector>
#include <memory>
#include <exception>
#include <stdexcept>
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
        return ptr() >= size();
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
    std::shared_ptr<Token> token()
    {
        throwIfOutOfRange();

        return getToken(ptr());
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
        m_tokens.clear();
    }

    // \brief Get size of total tokens currently being parsed
    // \returns Size of total tokens
    virtual size_t size() const noexcept // Declared virtual so that when inherited classes call one of our functions, our functions will call their overridden size()
    {
        return m_tokens.size();
    }

    // \brief Offsets used by the seek member
    typedef enum
    {
        Beg,
        Cur, // Read pointer
        RecentToken, // Position of most recent token received
        End
    } Position;

    // \brief Seek to a position
    // \param t_pos Position
    // \param t_off Offset
    // \returns Read pointer after seek
    // \note Does not range-check the final position
    size_t seek(const long long &t_pos, const size_t &t_off = Position::Beg) noexcept
    {
        m_readPtr = applyOffset(t_pos, t_off);
        
        return ptr();
    }

    // \brief Get a specific token
    // \param t_pos Position
    // \param t_off Offset
    // \returns Token at specified location
    // \throws std::range_error if the location is not in-range
    std::shared_ptr<Token> get(const long long &t_pos, const size_t &t_off = Position::Beg)
    {
        // Apply offset
        long long pos = applyOffset(t_pos, t_off);

        // Check range
        if (pos >= static_cast<long long int>(size()))
            throw std::range_error("Read pointer >= " + std::to_string(m_tokens.size()) + ".");

        return getToken(pos);
    }

protected:
    // \brief Return a token in the token array without range-checking the position first
    // \details Why? This makes it easier to inherit from this class
    virtual std::shared_ptr<Token> getToken(const size_t &t_pos) const
    {
        return m_tokens.at(t_pos);
    }
    

    long long applyOffset(const long long &t_pos, const size_t &t_off) const noexcept
    {
        long long fin = 0;
        
        switch (t_off)
        {
            case Position::Beg:
                fin = t_pos;
                break;
            
            case Position::Cur:
                fin = ptr() + t_pos - 1;
                [[fallthrough]];

            case Position::RecentToken:
                fin--; // The read pointer is always one position ahead of the previously received token from the parseToken() member
                break;

            case Position::End:
                fin = size() - 1 + t_pos;
        }
        
        return fin;
    }

    // \brief Check if `m_readPtr` is in-range
    // \throws std::range_error if `m_readPtr` is out-of-range
    void throwIfOutOfRange() const
    {
        if (ptr() >= size())
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
