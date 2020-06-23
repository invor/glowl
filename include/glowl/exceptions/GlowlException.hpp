/*
 * GlowlException.hpp
 *
 * MIT License
 * Copyright (c) 2020 Michael Becher, Moritz Heinemann
 */

#ifndef GLOWL_EXCEPTIONS_GLOWLEXCEPTION_HPP
#define GLOWL_EXCEPTIONS_GLOWLEXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace glowl
{
    class GlowlException : public std::exception
    {
    public:
        GlowlException(const std::string& message);
        virtual const char* what() const noexcept;

    private:
        std::string m_message;
    };

    inline GlowlException::GlowlException(const std::string& message) : m_message(message)
    {
    }

    inline const char* GlowlException::what() const noexcept
    {
        return m_message.c_str();
    }
} // namespace glowl

#endif // GLOWL_EXCEPTIONS_GLOWLEXCEPTION_HPP
