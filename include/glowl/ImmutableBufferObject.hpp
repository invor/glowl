/*
 * ImmutableBufferObject.hpp
 *
 * MIT License
 * Copyright (c) 2023 Michael Becher, Moritz Heinemann
 */

#ifndef GLOWL_IMMUTABLEBUFFEROBJECT_HPP
#define GLOWL_IMMUTABLEBUFFEROBJECT_HPP

#include "glinclude.h"

namespace glowl
{

    /**
     * \class ImmutableBufferObject
     *
     * \brief Generic OpenGL buffer object with immutable storage.
     *
     * \author Moritz Heinemann
     */
    class ImmutableBufferObject
    {
    private:
        GLuint     m_name;
        GLsizeiptr m_byte_size;

    public:
        /**
         * \brief ImmutableBufferObject constructor that uses std containers as input.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unique_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        template<typename Container>
        explicit ImmutableBufferObject(Container const& datastorage, GLbitfield flags = 0);

        /**
         * \brief ImmutableBufferObject constructor that uses data pointer and byte size as input.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unique_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        ImmutableBufferObject(GLvoid const* data, GLsizeiptr byte_size, GLbitfield flags = 0);

        ~ImmutableBufferObject();

        ImmutableBufferObject(const ImmutableBufferObject&) = delete;
        ImmutableBufferObject(ImmutableBufferObject&&) = delete;
        ImmutableBufferObject& operator=(ImmutableBufferObject&&) = delete;
        ImmutableBufferObject& operator=(const ImmutableBufferObject&) = delete;

        GLuint getName() const;

        GLsizeiptr getByteSize() const;

        void bind(GLenum target) const;

        void bindBase(GLenum target, GLuint index) const;

        static void copy(ImmutableBufferObject& src,
                         ImmutableBufferObject& tgt,
                         GLintptr               readOffset,
                         GLintptr               writeOffset,
                         GLsizeiptr             size);
    };

    template<typename Container>
    inline ImmutableBufferObject::ImmutableBufferObject(Container const& datastorage, GLbitfield flags)
        : m_name(0),
          m_byte_size(static_cast<GLsizeiptr>(datastorage.size() * sizeof(typename Container::value_type)))
    {
        glCreateBuffers(1, &m_name);
        glNamedBufferStorage(m_name, m_byte_size, datastorage.data(), flags);
    }

    inline ImmutableBufferObject::ImmutableBufferObject(GLvoid const* data, GLsizeiptr byte_size, GLbitfield flags)
        : m_name(0),
          m_byte_size(byte_size)
    {
        glCreateBuffers(1, &m_name);
        glNamedBufferStorage(m_name, m_byte_size, data, flags);
    }

    inline ImmutableBufferObject::~ImmutableBufferObject()
    {
        glDeleteBuffers(1, &m_name);
    }

    inline GLuint ImmutableBufferObject::getName() const
    {
        return m_name;
    }

    inline GLsizeiptr ImmutableBufferObject::getByteSize() const
    {
        return m_byte_size;
    }

    inline void ImmutableBufferObject::bind(GLenum target) const
    {
        glBindBuffer(target, m_name);
    }

    inline void ImmutableBufferObject::bindBase(GLenum target, GLuint index) const
    {
        glBindBufferBase(target, index, m_name);
    }

    inline void ImmutableBufferObject::copy(ImmutableBufferObject& src,
                                            ImmutableBufferObject& tgt,
                                            GLintptr               readOffset,
                                            GLintptr               writeOffset,
                                            GLsizeiptr             size)
    {
        glCopyNamedBufferSubData(src.m_name, tgt.m_name, readOffset, writeOffset, size);
    }

} // namespace glowl

#endif // GLOWL_IMMUTABLEBUFFEROBJECT_HPP
