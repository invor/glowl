/*
 * VertexLayout.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_VERTEXLAYOUT_HPP
#define GLOWL_VERTEXLAYOUT_HPP

#include "glinclude.h"

namespace glowl
{

    /**
     * \struct VertexLayout
     *
     * \brief Container for vertex layout descritions. Used by Mesh class.
     *
     * Usage hints (in combination with Mesh class):
     * Each VertexLayout instance is associated with exaclty one vertex buffer and describes the attributes
     * in that buffer. Thus, multiple vertex buffers require a matching number of layouts.
     * Give a correct stride value for the attributes (don't use 0 for tightly packed vertex data, DSA requires the
     * actual stride in bytes), e.g.:
     *
     * Non-interleaved, tightly packed vertex data,
     * e.g. two attribs in two buffers {vec3,vec3,...} and {vec2,vec2,...} have strides 12 and 8 in two diffrent vertex
     * layouts
     *
     * Partly interleaved (or non-tightly packed) vertex data,
     * e.g. three attribs in two buffers {vec3,vec3,...} and {{vec3,vec2},{vec3,vec2},...} have strides 12 and 20 in two
     * different vertex layouts
     *
     * Fully interleaved vertex data,
     * e.g. three attribs in one buffer {{vec3,vec3,vec2},{vec3,vec3,vec2},...} have stride 32 in a single vertex layout
     *
     * \author Michael Becher
     */
    struct VertexLayout
    {
        struct Attribute
        {
            Attribute(GLint     size,
                      GLenum    type,
                      GLboolean normalized,
                      GLsizei   offset,
                      GLenum    shader_input_type = GL_FLOAT)
                : size(size),
                  type(type),
                  normalized(normalized),
                  offset(offset),
                  shader_input_type(shader_input_type)
            {
            }

            GLint     size;
            GLenum    type;
            GLboolean normalized;
            GLsizei   offset;
            GLenum    shader_input_type; ///< type used by vertex shader input: float, double or integer
        };

        VertexLayout() : attributes() {}
        /**
         * Construct VertexLayout from set of strides and attributes
         *
         * \param strides Stride values in byte per vertex attribute. It is possible to use only a single stride value
         * for all attributes (see VertexLayout member documentation).
         *
         */
        VertexLayout(GLsizei stride, std::vector<Attribute> const& attributes) : stride(stride), attributes(attributes)
        {
        }
        /**
         * Construct VertexLayout from set of strides and attributes
         *
         * \param strides Stride values in byte per vertex attribute. It is possible to use only a single stride value
         * for all attributes (see VertexLayout member documentation).
         *
         */
        VertexLayout(GLsizei stride, std::vector<Attribute>&& attributes) : stride(stride), attributes(attributes) {}

        GLsizei                stride;
        std::vector<Attribute> attributes;
    };

    inline bool operator==(VertexLayout::Attribute const& lhs, VertexLayout::Attribute const& rhs)
    {
        return lhs.normalized == rhs.normalized && lhs.offset == rhs.offset && lhs.size == rhs.size &&
               lhs.type == rhs.type && lhs.shader_input_type == rhs.shader_input_type;
    }

    inline bool operator==(VertexLayout const& lhs, VertexLayout const& rhs)
    {
        bool rtn = true;

        rtn &= lhs.stride == rhs.stride;

        if (lhs.attributes.size() == rhs.attributes.size())
        {
            for (size_t i = 0; i < lhs.attributes.size(); ++i)
            {
                rtn &= (lhs.attributes[i] == rhs.attributes[i]);
            }
        }
        else
        {
            rtn = false;
        }

        return rtn;
    }

    static constexpr size_t computeByteSize(GLenum value_type)
    {
        size_t retval = 0;

        switch (value_type)
        {
        case GL_BYTE:
            retval = 1;
            break;
        case GL_SHORT:
            retval = 2;
            break;
        case GL_INT:
            retval = 4;
            break;
        case GL_FIXED:
            retval = 4;
            break;
        case GL_FLOAT:
            retval = 4;
            break;
        case GL_HALF_FLOAT:
            retval = 2;
            break;
        case GL_DOUBLE:
            retval = 8;
            break;
        case GL_UNSIGNED_BYTE:
            retval = 1;
            break;
        case GL_UNSIGNED_SHORT:
            retval = 2;
            break;
        case GL_UNSIGNED_INT:
            retval = 4;
            break;
        case GL_INT_2_10_10_10_REV:
            retval = 4;
            break;
        case GL_UNSIGNED_INT_2_10_10_10_REV:
            retval = 4;
            break;
        case GL_UNSIGNED_INT_10F_11F_11F_REV:
            retval = 4;
            break;
        default:
            break;
        }

        return retval;
    }

    static size_t computeAttributeByteSize(VertexLayout::Attribute attrib_desc)
    {
        return computeByteSize(attrib_desc.type) * attrib_desc.size;
    }

} // namespace glowl

#endif // GLOWL_VERTEXLAYOUT_HPP
