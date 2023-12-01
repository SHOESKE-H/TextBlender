#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token
{
public:
    Token() noexcept
        : m_type(0), m_value("") {}

    Token(const size_t &t_type) noexcept
        : m_type(t_type), m_value() {}

    Token(const size_t &t_type, const std::string &t_value) noexcept
        : m_type(t_type), m_value(t_value) {}

    size_t type() const noexcept
    {
        return m_type;
    }

    std::string value() const noexcept
    {
        return m_value;
    }

    void setType(const size_t &t_type) noexcept
    {
        m_type = t_type;
    }

    void setValue(const std::string &t_value) noexcept
    {
        m_value = t_value;
    }

private:
    size_t m_type; // Type 0 shall always be the 'Unknown' type
    std::string m_value;
};

#endif // TOKEN_H
