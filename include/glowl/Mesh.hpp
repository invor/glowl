/*
 * Mesh.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_MESH_HPP
#define GLOWL_MESH_HPP

// Include std libs
#include <memory>
#include <string>
#include <vector>

// Include glowl files
#include "BufferObject.hpp"
#include "VertexLayout.hpp"
#include "glinclude.h"

namespace glowl
{

    /**
     * \struct DrawElementsCommand
     *
     * \brief Convenience object for using indirect draw calls.
     *
     */
    struct DrawElementsCommand
    {
        GLuint cnt;
        GLuint instance_cnt;
        GLuint first_idx;
        GLuint base_vertex;
        GLuint base_instance;
    };

    /**
     * \class Mesh
     *
     * \brief Encapsulates mesh functionality.
     *
     * \author Michael Becher
     */
    class Mesh
    {
    public:
        typedef std::unique_ptr<BufferObject> BufferObjectPtr;

        /**
         * \brief Mesh constructor that requires data pointers and byte sizes as input.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        template<typename VertexPtr, typename IndexPtr>
        Mesh(std::vector<VertexPtr> const&    vertex_data,
             std::vector<size_t> const&       vertex_data_byte_sizes,
             IndexPtr const                   index_data,
             GLsizeiptr const                 index_data_byte_size,
             std::vector<VertexLayout> const& vertex_descriptor,
             GLenum const                     index_type = GL_UNSIGNED_INT,
             GLenum const                     usage = GL_STATIC_DRAW,
             GLenum const                     primitive_type = GL_TRIANGLES);

        /**
         * \brief Mesh constructor that requires std containers as input.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        template<typename VertexContainer, typename IndexContainer>
        Mesh(std::vector<VertexContainer> const& vertex_data,
             IndexContainer const&               index_data,
             std::vector<VertexLayout> const&    vertex_descriptor,
             GLenum const                        index_type = GL_UNSIGNED_INT,
             GLenum const                        usage = GL_STATIC_DRAW,
             GLenum const                        primitive_type = GL_TRIANGLES);

        ~Mesh()
        {
            glDeleteVertexArrays(1, &m_va_handle);
        }

        Mesh(const Mesh& cpy) = delete;
        Mesh(Mesh&& other) = delete;
        Mesh& operator=(Mesh&& rhs) = delete;
        Mesh& operator=(const Mesh& rhs) = delete;

        template<typename VertexContainer>
        void bufferVertexSubData(size_t vbo_idx, VertexContainer const& vertices, GLsizeiptr byte_offset);

        void bufferVertexSubData(size_t idx, GLvoid const* data, GLsizeiptr byte_size, GLsizeiptr byte_offset);

        template<typename IndexContainer>
        void bufferIndexSubData(IndexContainer const& indices, GLsizeiptr byte_offset);

        void bufferIndexSubData(GLvoid const* data, GLsizeiptr byte_size, GLsizeiptr byte_offset);

        void bindVertexArray() const
        {
            glBindVertexArray(m_va_handle);
        }

        /**
         * Draw function for your conveniences.
         * If you need/want to work with sth. different from glDrawElementsInstanced,
         * use bindVertexArray() and do your own thing.
         */
        void draw(GLsizei instance_cnt = 1)
        {
            glBindVertexArray(m_va_handle);
            glDrawElementsInstanced(m_primitive_type, m_indices_cnt, m_index_type, nullptr, instance_cnt);
            glBindVertexArray(0);
        }

        std::vector<VertexLayout> getVertexLayouts() const
        {
            return m_vertex_descriptor;
        }

        GLuint getIndicesCount() const
        {
            return m_indices_cnt;
        }

        GLenum getIndexType() const
        {
            return m_index_type;
        }

        GLenum getPrimitiveType() const
        {
            return m_primitive_type;
        }

        GLsizeiptr getVertexBufferByteSize(size_t vbo_idx) const
        {
            if (vbo_idx < m_vbos.size())
                return m_vbos[vbo_idx]->getByteSize();
            else
                return 0;
            // TODO: log some kind of error?
        }
        GLsizeiptr getIndexBufferByteSize() const
        {
            return m_ibo.getByteSize();
        }

        std::vector<BufferObjectPtr> const& getVbo() const
        {
            return m_vbos;
        }
        BufferObject const& getIbo() const
        {
            return m_ibo;
        }

    private:
        std::vector<BufferObjectPtr> m_vbos;
        BufferObject                 m_ibo;
        GLuint                       m_va_handle;

        std::vector<VertexLayout> m_vertex_descriptor;

        GLuint m_indices_cnt;
        GLenum m_index_type;
        GLenum m_usage;
        GLenum m_primitive_type;

        void createVertexArray();
    };

    template<typename VertexPtr, typename IndexPtr>
    inline Mesh::Mesh(std::vector<VertexPtr> const&    vertex_data,
                      std::vector<size_t> const&       vertex_data_byte_sizes,
                      IndexPtr const                   index_data,
                      GLsizeiptr const                 index_data_byte_size,
                      std::vector<VertexLayout> const& vertex_descriptor,
                      GLenum const                     indices_type,
                      GLenum const                     usage,
                      GLenum const                     primitive_type)
    try : m_ibo(GL_ELEMENT_ARRAY_BUFFER, index_data, index_data_byte_size, usage),
          m_va_handle(0),
          m_vertex_descriptor(vertex_descriptor),
          m_indices_cnt(0),
          m_index_type(indices_type),
          m_usage(usage),
          m_primitive_type(primitive_type)
    {
        try
        {
            for (unsigned int i = 0; i < vertex_data.size(); ++i)
            {
                m_vbos.emplace_back(
                    std::make_unique<BufferObject>(GL_ARRAY_BUFFER, vertex_data[i], vertex_data_byte_sizes[i], usage));
            }

            createVertexArray();
        }
        catch (...)
        {
            throw;
        }

        switch (m_index_type)
        {
        case GL_UNSIGNED_INT:
            m_indices_cnt = static_cast<GLuint>(index_data_byte_size / 4);
            break;
        case GL_UNSIGNED_SHORT:
            m_indices_cnt = static_cast<GLuint>(index_data_byte_size / 2);
            break;
        case GL_UNSIGNED_BYTE:
            m_indices_cnt = static_cast<GLuint>(index_data_byte_size / 1);
            break;
        }

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw MeshException("Mesh::Mesh - OpenGL error " + std::to_string(err));
        }
    }
    catch (...)
    {
        throw;
    }

    template<typename VertexContainer, typename IndexContainer>
    inline Mesh::Mesh(std::vector<VertexContainer> const& vertex_data,
                      IndexContainer const&               index_data,
                      std::vector<VertexLayout> const&    vertex_descriptor,
                      GLenum                              indices_type,
                      GLenum                              usage,
                      GLenum                              primitive_type)
    try : m_ibo(GL_ELEMENT_ARRAY_BUFFER,
                index_data,
                usage), // TODO ibo generation in constructor might fail? needs a bound vao?
          m_va_handle(0),
          m_vertex_descriptor(vertex_descriptor),
          m_indices_cnt(0),
          m_index_type(indices_type),
          m_usage(usage),
          m_primitive_type(primitive_type)
    {
        // Some sanity checks
        // TODO check if vertex buffer count matches attribute count, throw exception if not?
        try
        {
            for (unsigned int i = 0; i < vertex_data.size(); ++i)
                m_vbos.emplace_back(std::make_unique<BufferObject>(GL_ARRAY_BUFFER, vertex_data[i], m_usage));
        }
        catch (...)
        {
            throw;
        }

        createVertexArray();

        GLuint vi_size = static_cast<GLuint>(index_data.size() * sizeof(typename IndexContainer::value_type));

        switch (m_index_type)
        {
        case GL_UNSIGNED_INT:
            m_indices_cnt = static_cast<GLuint>(vi_size / 4);
            break;
        case GL_UNSIGNED_SHORT:
            m_indices_cnt = static_cast<GLuint>(vi_size / 2);
            break;
        case GL_UNSIGNED_BYTE:
            m_indices_cnt = static_cast<GLuint>(vi_size / 1);
            break;
        }

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw MeshException("Mesh::Mesh - OpenGL error " + std::to_string(err));
        }
    }
    catch (...)
    {
        throw;
    }

    template<typename VertexContainer>
    inline void Mesh::bufferVertexSubData(size_t vbo_idx, VertexContainer const& vertices, GLsizeiptr byte_offset)
    {
        if (vbo_idx < m_vbos.size())
        {
            try
            {
                m_vbos[vbo_idx]->bufferSubData<VertexContainer>(vertices, byte_offset);
            }
            catch (...)
            {
                throw;
            }
        }
        else
        {
            throw MeshException("Mesh::bufferVertexSubData - vertex buffer index out of range");
        }
    }

    inline void Mesh::bufferVertexSubData(size_t vbo_idx, GLvoid const* data, GLsizeiptr byte_size, GLsizeiptr byte_offset)
    {
        if (vbo_idx < m_vbos.size())
        {
            try
            {
                m_vbos[vbo_idx]->bufferSubData(data, byte_size, byte_offset);
            }
            catch (...)
            {
                throw;
            }
        }
        else
        {
            throw MeshException("Mesh::bufferVertexSubData - vertex buffer index out of range");
        }
    }

    template<typename IndexContainer>
    inline void Mesh::bufferIndexSubData(IndexContainer const& indices, GLsizeiptr byte_offset)
    {
        try
        {
            m_ibo.bufferSubData<IndexContainer>(indices, byte_offset);
        }
        catch (...)
        {
            throw;
        }
    }

    inline void Mesh::bufferIndexSubData(GLvoid const* data, GLsizeiptr byte_size, GLsizeiptr byte_offset)
    {
        try
        {
            m_ibo.bufferSubData(data, byte_size, byte_offset);
        }
        catch (...)
        {
            throw;
        }
    }

    inline void Mesh::createVertexArray()
    {
        glCreateVertexArrays(1, &m_va_handle);

        GLuint attrib_idx = 0;

        for (size_t vertex_layout_idx = 0; vertex_layout_idx < m_vertex_descriptor.size(); ++vertex_layout_idx)
        {
            glVertexArrayVertexBuffer(m_va_handle,
                                      vertex_layout_idx,
                                      m_vbos[vertex_layout_idx]->getName(),
                                      0, // offset not really needed since we just created a new vbo
                                      m_vertex_descriptor[vertex_layout_idx].stride);

            for (size_t local_attrib_idx = 0;
                 local_attrib_idx < m_vertex_descriptor[vertex_layout_idx].attributes.size();
                 ++local_attrib_idx)
            {
                auto const& attribute = m_vertex_descriptor[vertex_layout_idx].attributes[local_attrib_idx];

                glEnableVertexArrayAttrib(m_va_handle, attrib_idx);
                switch (attribute.shader_input_type)
                {
                case GL_FLOAT:
                    glVertexArrayAttribFormat(m_va_handle,
                                              attrib_idx,
                                              attribute.size,
                                              attribute.type,
                                              attribute.normalized,
                                              attribute.offset);
                    break;
                case GL_INT:
                    glVertexArrayAttribIFormat(m_va_handle,
                                              attrib_idx,
                                              attribute.size,
                                              attribute.type,
                                              attribute.offset);
                    break;
                case GL_DOUBLE:
                    glVertexArrayAttribLFormat(m_va_handle,
                                              attrib_idx,
                                              attribute.size,
                                              attribute.type,
                                              attribute.offset);
                    break;
                default:
                    throw MeshException("Mesh::createVertexArray - invalid vertex shader input type given (use float, double or int)");
                    break;
                }
                glVertexArrayAttribBinding(m_va_handle, attrib_idx, vertex_layout_idx);

                ++attrib_idx;
            }
        }

        glVertexArrayElementBuffer(m_va_handle, m_ibo.getName());
    }

} // namespace glowl

#endif // GLOWL_MESH_HPP
