/*
 * BufferObject.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_BUFFEROBJECT_HPP
#define GLOWL_BUFFEROBJECT_HPP

#include <glad/glad.h>

#include <iostream>

namespace glowl
{

    /**
     * \class BufferObject
     *
     * \brief Generic OpenGL buffer object.
     *
     * \author Michael Becher
     */
    class BufferObject
    {
    public:
        /**
         * \brief BufferObject constructor that uses std containers as input.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        template<typename Container>
        BufferObject(GLenum target, Container const& datastorage, GLenum usage = GL_DYNAMIC_DRAW);

        /**
         * \brief Bufferobject constructor that uses data pointer and byte size as input.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        BufferObject(GLenum target, GLvoid const* data, GLsizeiptr byte_size, GLenum usage = GL_DYNAMIC_DRAW);

        ~BufferObject();

        BufferObject(const BufferObject& cpy) = delete;
        BufferObject(BufferObject&& other) = delete;
        BufferObject& operator=(BufferObject&& rhs) = delete;
        BufferObject& operator=(const BufferObject& rhs) = delete;

        template<typename Container>
        void bufferSubData(Container const& datastorage, GLsizeiptr byte_offset = 0) const;

        void bufferSubData(GLvoid const* data, GLsizeiptr byte_size, GLsizeiptr byte_offset = 0) const;

        template<typename Container>
        void rebuffer(Container const& datastorage);

        void rebuffer(GLvoid const* data, GLsizeiptr byte_size);

        void bind() const;

        void bind(GLuint index) const;

        void bindAs(GLenum target, GLuint index) const;

        static void copy(BufferObject* src, BufferObject* tgt);

        static void copy(BufferObject* src,
                         BufferObject* tgt,
                         GLintptr      readOffset,
                         GLintptr      writeOffset,
                         GLsizeiptr    size);

        GLenum getTarget() const;

        GLsizeiptr getByteSize() const;

    private:
        GLenum     m_target;
        GLuint     m_handle;
        GLsizeiptr m_byte_size;
        GLenum     m_usage;
    };

    template<typename Container>
    inline BufferObject::BufferObject(GLenum target, Container const& datastorage, GLenum usage)
        : m_target(target),
          m_handle(0),
          m_byte_size(static_cast<GLsizeiptr>(datastorage.size() * sizeof(typename Container::value_type))),
          m_usage(usage)
    {
        glGenBuffers(1, &m_handle);
        glBindBuffer(m_target, m_handle);
        glBufferData(m_target, m_byte_size, datastorage.data(), m_usage);
        glBindBuffer(m_target, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "Error - BufferObject - Construction: " << err << std::endl;
        }
    }

    inline BufferObject::BufferObject(GLenum target, GLvoid const* data, GLsizeiptr byte_size, GLenum usage)
        : m_target(target), m_handle(0), m_byte_size(byte_size), m_usage(usage)
    {
        glGenBuffers(1, &m_handle);
        glBindBuffer(m_target, m_handle);
        glBufferData(m_target, m_byte_size, data, m_usage);
        glBindBuffer(m_target, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "Error - BufferObject - Construction: " << err << std::endl;
        }
    }

    inline BufferObject::~BufferObject()
    {
        glDeleteBuffers(1, &m_handle);
    }

    template<typename Container>
    inline void BufferObject::bufferSubData(Container const& datastorage, GLsizeiptr byte_offset) const
    {
        // check if feasible
        if ((byte_offset + static_cast<GLsizeiptr>(datastorage.size() * sizeof(typename Container::value_type))) >
            m_byte_size)
        {
            // error message
            std::cerr << "Error - BufferObject - bufferSubData: given data too large for buffer." << std::endl;
            return;
        }

        glBindBuffer(m_target, m_handle);
        glBufferSubData(
            m_target, byte_offset, datastorage.size() * sizeof(typename Container::value_type), datastorage.data());
        glBindBuffer(m_target, 0);
    }

    inline void BufferObject::bufferSubData(GLvoid const* data, GLsizeiptr byte_size, GLsizeiptr byte_offset) const
    {
        // check if feasible
        if ((byte_offset + byte_size) > m_byte_size)
        {
            // error message
            std::cerr << "Error - BufferObject - bufferSubData: given data too large for buffer." << std::endl;
            return;
        }

        glBindBuffer(m_target, m_handle);
        glBufferSubData(m_target, byte_offset, byte_size, data);
        glBindBuffer(m_target, 0);
    }

    template<typename Container>
    inline void BufferObject::rebuffer(Container const& datastorage)
    {
        m_byte_size = static_cast<GLsizeiptr>(datastorage.size() * sizeof(typename Container::value_type));
        glBindBuffer(m_target, m_handle);
        glBufferData(m_target, m_byte_size, datastorage.data(), m_usage);
        glBindBuffer(m_target, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "Error - BufferObject - rebuffer: " << err << std::endl;
        }
    }

    inline void BufferObject::rebuffer(GLvoid const* data, GLsizeiptr byte_size)
    {
        m_byte_size = byte_size;
        glBindBuffer(m_target, m_handle);
        glBufferData(m_target, m_byte_size, data, m_usage);
        glBindBuffer(m_target, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "Error - BufferObject - rebuffer: " << err << std::endl;
        }
    }

    inline void BufferObject::bind() const
    {
        glBindBuffer(m_target, m_handle);
    }

    inline void BufferObject::bind(GLuint index) const
    {
        glBindBufferBase(m_target, index, m_handle);
    }

    inline void BufferObject::bindAs(GLenum target, GLuint index) const
    {
        glBindBufferBase(target, index, m_handle);
        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "Error - BufferObject - rebindAs: " << err << std::endl;
        }
    }

    inline void BufferObject::copy(BufferObject* src, BufferObject* tgt)
    {
        if (src->m_byte_size > tgt->m_byte_size)
        {
            std::cerr << "Error: ShaderStorageBufferObject::copy - target buffer smaller than source." << std::endl;
            return;
        }

        glBindBuffer(GL_COPY_READ_BUFFER, src->m_handle);
        glBindBuffer(GL_COPY_WRITE_BUFFER, tgt->m_handle);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, src->m_byte_size);

        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }

    inline void BufferObject::copy(BufferObject* src,
                                   BufferObject* tgt,
                                   GLintptr      readOffset,
                                   GLintptr      writeOffset,
                                   GLsizeiptr    size)
    {
        if ((readOffset + size) > src->m_byte_size)
        {
            // std::cerr << "Error: ShaderStorageBufferObject::copy - target buffer smaller than source." <<
            // std::endl;
            return;
        }
        else if ((writeOffset + size) > tgt->m_byte_size)
        {
            return;
        }

        glBindBuffer(GL_COPY_READ_BUFFER, src->m_handle);
        glBindBuffer(GL_COPY_WRITE_BUFFER, tgt->m_handle);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);

        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }

    inline GLenum BufferObject::getTarget() const
    {
        return m_target;
    }

    inline GLsizeiptr BufferObject::getByteSize() const
    {
        return m_byte_size;
    }

} // namespace glowl

#endif // GLOWL_BUFFEROBJECT_HPP
