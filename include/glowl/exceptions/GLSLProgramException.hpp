/*
 * GLSLProgramException.hpp
 *
 * MIT License
 * Copyright (c) 2020 Michael Becher, Moritz Heinemann
 */

#ifndef GLOWL_EXCEPTIONS_GLSLPROGRAMEXCEPTION_HPP
#define GLOWL_EXCEPTIONS_GLSLPROGRAMEXCEPTION_HPP

#include "GlowlException.hpp"

namespace glowl
{
    class GLSLProgramException : public GlowlException
    {
    public:
        using GlowlException::GlowlException;
    };
} // namespace glowl

#endif // GLOWL_EXCEPTIONS_GLSLPROGRAMEXCEPTION_HPP
