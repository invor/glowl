/*
 * GLSLProgramException.hpp
 *
 * MIT License
 * Copyright (c) 2020 Michael Becher, Moritz Heinemann
 */

#ifndef GLOWL_GLSLPROGRAMEXCEPTION_HPP
#define GLOWL_GLSLPROGRAMEXCEPTION_HPP

#include "BaseException.hpp"

namespace glowl
{
    class GLSLProgramException : public BaseException
    {
    public:
        using BaseException::BaseException;
    };
} // namespace glowl

#endif // GLOWL_GLSLPROGRAMEXCEPTION_HPP
