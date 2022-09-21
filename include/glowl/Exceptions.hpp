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
    // Use a common base class for all glowl exceptions to allow combined catching.
    class BaseException : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    class BufferObjectException : public BaseException
    {
    public:
        using BaseException::BaseException;
    };

    class FramebufferObjectException : public BaseException
    {
    public:
        using BaseException::BaseException;
    };

    class GLSLProgramException : public BaseException
    {
    public:
        using BaseException::BaseException;
    };

    class MeshException : public BaseException
    {
    public:
        using BaseException::BaseException;
    };

    class TextureException : public BaseException
    {
    public:
        using BaseException::BaseException;
    };

    class SamplerException : public BaseException
    {
    public:
        using BaseException::BaseException;
    };
} // namespace glowl

#endif // GLOWL_EXCEPTIONS_HPP
