/*
 * VertexLayout.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef VertexLayout_hpp
#define VertexLayout_hpp

#include <glad/glad.h>

namespace glowl
{

    /**
    * \struct VertexLayout
    *
    * \brief Container for vertex layout descritions. Used by Mesh class.
    *
    * \author Michael Becher
    */
    struct VertexLayout
    {
        struct Attribute
        {
            Attribute(GLint size, GLenum type, GLboolean normalized, GLsizei offset)
                : size(size), type(type), normalized(normalized), offset(offset) {}

            GLint size;
            GLenum type;
            GLboolean normalized;
            GLsizei offset;
        };

        VertexLayout() : strides(), attributes() {}
        /**
         * Construct VertexLayout from set of strides and attributes
         * 
         * \param strides Stride values in byte per vertex attribute. It is possible to use only a single stride value for all attributes (see VertexLayout member documentation).
         *
         */
        VertexLayout(std::vector<GLsizei> const& strides, std::vector<Attribute> const& attributes)
            : strides(strides), attributes(attributes) {}
        /**
         * Construct VertexLayout from set of strides and attributes
         *
         * \param strides Stride values in byte per vertex attribute. It is possible to use only a single stride value for all attributes (see VertexLayout member documentation).
         *
         */
        VertexLayout(std::vector<GLsizei>&& strides, std::vector<Attribute>&& attributes)
            : strides(strides), attributes(attributes) {}

        /**
         * \brief Stride values in byte per vertex attributes.
         * 
         * Usage hints (in combination with Mesh class):
         *
         * Non-interleaved, tightly packed vertex data, i.e. multiple attributes, each with its own separate vertex buffer with tightly packed memory:
         * Either a stride of 0 per attribute, e.g. {0,0,..}, or a single stride value of 0 that is used for all attributes, e.g. {0}
         *
         * Partly interleaved or NON-tightly packed vertex data, i.e. multiple attributes, split across multiple vertex buffer with indivdual strides or potentially with padded memory:
         * Give a correct stride value for each! attribute, e.g. {12,20,20} for three attribs in two buffers {vec3,vec3,...} and {{vec3,vec2},{vec3,vec2},...}
         *
         * Fully interleaved vertex data, i.e. multiple attributes within a single vertex buffer:
         * Either a stride of sizeof(Vertex) per attribute, e.g. {32,32,32} for {{vec3,vec3,vec2},{vec3,vec3,vec2},...}, or a single stride value of sizeof(Vertex) that is used for all attributes, e.g. {32}
         *
         */
        std::vector<GLsizei>   strides;
        std::vector<Attribute> attributes;
    };

    inline
        bool operator==(VertexLayout::Attribute const& lhs, VertexLayout::Attribute const& rhs)
    {
        return lhs.normalized == rhs.normalized && lhs.offset == rhs.offset && lhs.size == rhs.size && lhs.type == rhs.type;
    }

    inline
        bool operator==(VertexLayout const& lhs, VertexLayout const& rhs)
    {
        bool rtn = false;

        if (lhs.strides.size() == rhs.strides.size())
        {
            for (size_t i = 0; i < lhs.strides.size(); ++i)
            {
                rtn &= (lhs.strides[i] == rhs.strides[i]);
            }
        }
        else
        {
            rtn = false;
        }

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

}

#endif // !VertexLayout_hpp
