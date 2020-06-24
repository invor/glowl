/*
 * Exceptions.hpp
 *
 * MIT License
 * Copyright (c) 2020 Michael Becher, Moritz Heinemann
 */

#ifndef GLOWL_EXCEPTIONS_HPP
#define GLOWL_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace glowl
{
    class BaseException : public std::exception
    {
    public:
        BaseException(std::string const& message) : m_message(message) {}
        virtual const char* what() const noexcept
        {
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };

    class GLSLProgramException : public BaseException
    {
    public:
        using BaseException::BaseException;
    };
} // namespace glowl

#endif // GLOWL_EXCEPTIONS_HPP
