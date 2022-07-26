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
    class BufferObjectException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class FramebufferObjectException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class GLSLProgramException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class MeshException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };

    class TextureException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
} // namespace glowl

#endif // GLOWL_EXCEPTIONS_HPP
