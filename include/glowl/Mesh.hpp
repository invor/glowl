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

        using VertexPtrData = std::tuple<void const*, std::size_t, VertexLayout>;

        using VertexPtrDataList = std::vector<VertexPtrData>;

        template<typename VertexDataType>
        using VertexData = std::pair<std::vector<VertexDataType>, VertexLayout>;

        template<typename VertexDataType>
        using VertexDataList = std::vector<VertexData<VertexDataType>>;

        /**
         * \brief Mesh constructor that requires data pointers and byte sizes as input.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        Mesh(std::vector<void const*> const&  vertex_data,
             std::vector<std::size_t> const&  vertex_data_byte_sizes,
             std::vector<VertexLayout> const& vertex_descriptor,
             void const*                      index_data,
             std::size_t const                index_data_byte_size,
             GLenum const                     index_type = GL_UNSIGNED_INT,
             GLenum const                     primitive_type = GL_TRIANGLES,
             GLenum const                     usage = GL_STATIC_DRAW);

        /**
         * \brief Mesh constructor that requires data pointers and byte sizes as input.
         *
         * \param vertex_data A vector where each entry contains (in a tuple) a pointer to vertex data,
         *  the byte size of that data and the corresponding VertexLayout
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        Mesh(VertexPtrDataList const& vertex_data,
             void const*              index_data,
             std::size_t const        index_data_byte_size,
             GLenum const             index_type = GL_UNSIGNED_INT,
             GLenum const             primitive_type = GL_TRIANGLES,
             GLenum const             usage = GL_STATIC_DRAW);

        /**
         * \brief Mesh constructor that requires data in std vectors as input.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        template<typename VertexDataType, typename IndexDataType>
        Mesh(std::vector<std::vector<VertexDataType>> const& vertex_data,
             std::vector<VertexLayout> const&                vertex_descriptor,
             std::vector<IndexDataType> const&               index_data,
             GLenum const                                    index_type = GL_UNSIGNED_INT,
             GLenum const                                    primitive_type = GL_TRIANGLES,
             GLenum const                                    usage = GL_STATIC_DRAW);

        /**
         * \brief Mesh constructor that requires data in std vectors as input.
         *
         * \param vertex_data A vector where each entry contains (in a pair) the vertex data in a std vector and the
         * corresponding VertexLayout.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        template<typename VertexDataType, typename IndexDataType>
        Mesh(VertexDataList<VertexDataType> const& vertex_data,
             std::vector<IndexDataType> const&     index_data,
             GLenum const                          index_type = GL_UNSIGNED_INT,
             GLenum const                          primitive_type = GL_TRIANGLES,
             GLenum const                          usage = GL_STATIC_DRAW);

        ~Mesh()
        {
            glDeleteVertexArrays(1, &m_va_handle);
        }

        Mesh(const Mesh& cpy) = delete;
        Mesh(Mesh&& other) = delete;
        Mesh& operator=(Mesh&& rhs) = delete;
        Mesh& operator=(const Mesh& rhs) = delete;

        template<typename VertexDataType>
        void bufferVertexSubData(std::size_t                        vbo_idx,
                                 std::vector<VertexDataType> const& vertices,
                                 GLsizeiptr                         byte_offset);

        void bufferVertexSubData(std::size_t vbo_idx, GLvoid const* data, GLsizeiptr byte_size, GLsizeiptr byte_offset);

        template<typename IndexDataType>
        void bufferIndexSubData(std::vector<IndexDataType> const& indices, GLsizeiptr byte_offset);

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

        GLsizeiptr getVertexBufferByteSize(std::size_t vbo_idx) const
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

        std::vector<BufferObjectPtr> const& getVbos() const
        {
            return m_vbos;
        }

        BufferObject const& getIbo() const
        {
            return m_ibo;
        }

    private:
        GLuint                       m_va_handle;
        std::vector<BufferObjectPtr> m_vbos;
        BufferObject                 m_ibo;

        std::vector<VertexLayout> m_vertex_descriptor;

        GLuint m_indices_cnt;
        GLenum m_index_type;
        GLenum m_primitive_type;
        GLenum m_usage;

        void createVertexArray();
        void setIndicesCount(GLuint index_data_byte_size);
        void checkError();
    };

    inline Mesh::Mesh(std::vector<void const*> const&  vertex_data,
                      std::vector<std::size_t> const&  vertex_data_byte_sizes,
                      std::vector<VertexLayout> const& vertex_descriptor,
                      void const*                      index_data,
                      std::size_t const                index_data_byte_size,
                      GLenum const                     index_type,
                      GLenum const                     primitive_type,
                      GLenum const                     usage)
        : m_va_handle(0),
          m_ibo(GL_ELEMENT_ARRAY_BUFFER, index_data, index_data_byte_size, usage),
          m_vertex_descriptor(vertex_descriptor),
          m_indices_cnt(0),
          m_index_type(index_type),
          m_primitive_type(primitive_type),
          m_usage(usage)
    {
        if (vertex_data.size() != vertex_data_byte_sizes.size() || vertex_data.size() != vertex_descriptor.size())
        {
            throw std::invalid_argument("Mesh::Mesh - Vector parameters of different size!");
        }

        for (unsigned int i = 0; i < vertex_data.size(); ++i)
        {
            m_vbos.emplace_back(
                std::make_unique<BufferObject>(GL_ARRAY_BUFFER, vertex_data[i], vertex_data_byte_sizes[i], usage));
        }

        createVertexArray();
        setIndicesCount(index_data_byte_size);

        checkError();
    }

    inline Mesh::Mesh(VertexPtrDataList const& vertex_data,
                      void const*              index_data,
                      std::size_t const        index_data_byte_size,
                      GLenum const             index_type,
                      GLenum const             primitive_type,
                      GLenum const             usage)
        : m_va_handle(0),
          m_ibo(GL_ELEMENT_ARRAY_BUFFER, index_data, index_data_byte_size, usage),
          m_vertex_descriptor(),
          m_indices_cnt(0),
          m_index_type(index_type),
          m_primitive_type(primitive_type),
          m_usage(usage)
    {
        for (unsigned int i = 0; i < vertex_data.size(); ++i)
        {
            m_vbos.emplace_back(std::make_unique<BufferObject>(GL_ARRAY_BUFFER,
                                                               std::get<0>(vertex_data[i]),
                                                               std::get<1>(vertex_data[i]),
                                                               usage));
            m_vertex_descriptor.push_back(std::get<2>(vertex_data[i]));
        }

        createVertexArray();
        setIndicesCount(index_data_byte_size);

        checkError();
    }

    template<typename VertexDataType, typename IndexDataType>
    inline Mesh::Mesh(std::vector<std::vector<VertexDataType>> const& vertex_data,
                      std::vector<VertexLayout> const&                vertex_descriptor,
                      std::vector<IndexDataType> const&               index_data,
                      GLenum const                                    index_type,
                      GLenum const                                    primitive_type,
                      GLenum const                                    usage)
        : m_va_handle(0),
          m_ibo(GL_ELEMENT_ARRAY_BUFFER,
                index_data,
                usage), // TODO ibo generation in constructor might fail? needs a bound vao?
          m_vertex_descriptor(vertex_descriptor),
          m_indices_cnt(0),
          m_index_type(index_type),
          m_primitive_type(primitive_type),
          m_usage(usage)
    {
        if (vertex_data.size() != vertex_descriptor.size())
        {
            throw std::invalid_argument("Mesh::Mesh - Vector parameters of different size!");
        }

        for (unsigned int i = 0; i < vertex_data.size(); ++i)
        {
            m_vbos.emplace_back(std::make_unique<BufferObject>(GL_ARRAY_BUFFER, vertex_data[i], m_usage));
        }

        createVertexArray();

        GLuint vi_size =
            static_cast<GLuint>(index_data.size() * sizeof(typename std::vector<IndexDataType>::value_type));
        setIndicesCount(vi_size);

        checkError();
    }

    template<typename VertexDataType, typename IndexDataType>
    inline Mesh::Mesh(VertexDataList<VertexDataType> const& vertex_data_list,
                      std::vector<IndexDataType> const&     index_data,
                      GLenum const                          index_type,
                      GLenum const                          primitive_type,
                      GLenum const                          usage)
        : m_va_handle(0),
          m_ibo(GL_ELEMENT_ARRAY_BUFFER, index_data, usage),
          m_indices_cnt(0),
          m_index_type(index_type),
          m_primitive_type(primitive_type),
          m_usage(usage)
    {
        for (auto const& vertex_data : vertex_data_list)
        {
            m_vbos.emplace_back(std::make_unique<BufferObject>(GL_ARRAY_BUFFER, vertex_data.first, m_usage));
            m_vertex_descriptor.push_back(vertex_data.second);
        }

        createVertexArray();

        GLuint vi_size =
            static_cast<GLuint>(index_data.size() * sizeof(typename std::vector<IndexDataType>::value_type));
        setIndicesCount(vi_size);

        checkError();
    }

    template<typename VertexDataType>
    inline void Mesh::bufferVertexSubData(std::size_t                        vbo_idx,
                                          std::vector<VertexDataType> const& vertices,
                                          GLsizeiptr                         byte_offset)
    {
        if (vbo_idx >= m_vbos.size())
        {
            throw MeshException("Mesh::bufferVertexSubData - vertex buffer index out of range");
        }
        m_vbos[vbo_idx]->bufferSubData<std::vector<VertexDataType>>(vertices, byte_offset);
    }

    inline void Mesh::bufferVertexSubData(std::size_t   vbo_idx,
                                          GLvoid const* data,
                                          GLsizeiptr    byte_size,
                                          GLsizeiptr    byte_offset)
    {
        if (vbo_idx >= m_vbos.size())
        {
            throw MeshException("Mesh::bufferVertexSubData - vertex buffer index out of range");
        }
        m_vbos[vbo_idx]->bufferSubData(data, byte_size, byte_offset);
    }

    template<typename IndexDataType>
    inline void Mesh::bufferIndexSubData(std::vector<IndexDataType> const& indices, GLsizeiptr byte_offset)
    {
        m_ibo.bufferSubData<std::vector<IndexDataType>>(indices, byte_offset);
    }

    inline void Mesh::bufferIndexSubData(GLvoid const* data, GLsizeiptr byte_size, GLsizeiptr byte_offset)
    {
        m_ibo.bufferSubData(data, byte_size, byte_offset);
    }

    inline void Mesh::createVertexArray()
    {
        glCreateVertexArrays(1, &m_va_handle);

        GLuint attrib_idx = 0;

        for (std::size_t vertex_layout_idx = 0; vertex_layout_idx < m_vertex_descriptor.size(); ++vertex_layout_idx)
        {
            glVertexArrayVertexBuffer(m_va_handle,
                                      vertex_layout_idx,
                                      m_vbos[vertex_layout_idx]->getName(),
                                      0, // offset not really needed since we just created a new vbo
                                      m_vertex_descriptor[vertex_layout_idx].stride);

            for (std::size_t local_attrib_idx = 0;
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
                    throw MeshException(
                        "Mesh::createVertexArray - invalid vertex shader input type given (use float, double or int)");
                    break;
                }
                glVertexArrayAttribBinding(m_va_handle, attrib_idx, vertex_layout_idx);

                ++attrib_idx;
            }
        }

        glVertexArrayElementBuffer(m_va_handle, m_ibo.getName());
    }

    inline void Mesh::setIndicesCount(GLuint index_data_byte_size)
    {
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
    }

    inline void Mesh::checkError()
    {
        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw MeshException("Mesh::Mesh - OpenGL error " + std::to_string(err));
        }
    }

} // namespace glowl

#endif // GLOWL_MESH_HPP
