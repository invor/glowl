/*
 * BaseException.hpp
 *
 * MIT License
 * Copyright (c) 2020 Michael Becher, Moritz Heinemann
 */

#ifndef GLOWL_BASEEXCEPTION_HPP
#define GLOWL_BASEEXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace glowl
{
    class BaseException : public std::exception
    {
    public:
        BaseException(std::string const& message);
        virtual const char* what() const noexcept;

    private:
        std::string m_message;
    };

    inline BaseException::BaseException(std::string const& message) : m_message(message)
    {
    }

    inline const char* BaseException::what() const noexcept
    {
        return m_message.c_str();
    }
} // namespace glowl

#endif // GLOWL_BASEEXCEPTION_HPP
