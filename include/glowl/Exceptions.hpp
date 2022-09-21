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
    class GlowlException : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    class BufferObjectException : public GlowlException
    {
    public:
        using GlowlException::GlowlException;
    };

    class FramebufferObjectException : public GlowlException
    {
    public:
        using GlowlException::GlowlException;
    };

    class GLSLProgramException : public GlowlException
    {
    public:
        using GlowlException::GlowlException;
    };

    class MeshException : public GlowlException
    {
    public:
        using GlowlException::GlowlException;
    };

    class TextureException : public GlowlException
    {
    public:
        using GlowlException::GlowlException;
    };
} // namespace glowl

#endif // GLOWL_EXCEPTIONS_HPP
