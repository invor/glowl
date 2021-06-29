/*
 * Texture3D.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_TEXTURE3D_HPP
#define GLOWL_TEXTURE3D_HPP

#include "Exceptions.hpp"
#include "Texture.hpp"

namespace glowl
{

    /**
     * \class Texture3D
     *
     * \brief Encapsulates basic 3D texure functionality.
     *
     * This class encapsulates basic 3D functionality including creation of a 3D texture,
     * texture updates and texture binding.
     *
     * \author Michael Becher
     */
    class Texture3D : public Texture
    {
    public:
        /**
         * \brief Texture3D constructor.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        Texture3D(std::string          id,
                  TextureLayout const& layout,
                  GLvoid const*        data,
                  bool                 generateMipmap = false,
                  bool                 customLevels = false);
        Texture3D(const Texture3D&) = delete;
        Texture3D(Texture3D&& other) = delete;
        Texture3D& operator=(const Texture3D& rhs) = delete;
        Texture3D& operator=(Texture3D&& rhs) = delete;
        ~Texture3D();

        /**
         * \brief Bind the texture.
         */
        void bindTexture() const;

        void updateMipmaps();

        /**
         * \brief Reload the texture.
         * \param data Pointer to the new texture data.
         */
        void reload(TextureLayout const& layout,
                    GLvoid const*        data,
                    bool                 generateMipmap = false,
                    bool                 customLevels = false);

        TextureLayout getTextureLayout() const;

        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getDepth();

    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_depth;
    };

    inline Texture3D::Texture3D(std::string          id,
                                TextureLayout const& layout,
                                GLvoid const*        data,
                                bool                 generateMipmap,
                                bool                 customLevels)
        : Texture(id, layout.internal_format, layout.format, layout.type, layout.levels),
          m_width(layout.width),
          m_height(layout.height),
          m_depth(layout.depth)
    {
        glCreateTextures(GL_TEXTURE_3D, 1, &m_name);

        for (auto& pname_pvalue : layout.int_parameters)
        {
            glTextureParameteri(m_name, pname_pvalue.first, pname_pvalue.second);
        }

        for (auto& pname_pvalue : layout.float_parameters)
        {
            glTextureParameterf(m_name, pname_pvalue.first, pname_pvalue.second);
        }

        if (generateMipmap && !customLevels)
        {
            m_levels = 1 + static_cast<GLsizei>(std::floor(std::log2(std::max(m_depth, std::max(m_width, m_height)))));
        }

        glTextureStorage3D(m_name, m_levels, m_internal_format, m_width, m_height, m_depth);

        if (data != nullptr)
        {
            glTextureSubImage3D(m_name, 0, 0, 0, 0, m_width, m_height, m_depth, m_format, m_type, data);
        }

        if (generateMipmap)
        {
            glGenerateTextureMipmap(m_name);
        }

#ifndef GLOWL_NO_ARB_BINDLESS_TEXTURE
        m_texture_handle = glGetTextureHandleARB(m_name);
#endif

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture3D::Texture3D - texture id: " + m_id + " - OpenGL error " +
                                   std::to_string(err));
        }
    }

    inline Texture3D::~Texture3D()
    {
        glDeleteTextures(1, &m_name);
    }

    inline void Texture3D::bindTexture() const
    {
        glBindTexture(GL_TEXTURE_3D, m_name);
    }

    inline void Texture3D::updateMipmaps()
    {
        glGenerateTextureMipmap(m_name);
    }

    inline void Texture3D::reload(TextureLayout const& layout,
                                  GLvoid const*        data,
                                  bool                 generateMipmap,
                                  bool                 customLevels)
    {
        m_width = layout.width;
        m_height = layout.height;
        m_depth = layout.depth;
        m_internal_format = layout.internal_format;
        m_format = layout.format;
        m_type = layout.type;

        glDeleteTextures(1, &m_name);

        glCreateTextures(GL_TEXTURE_3D, 1, &m_name);

        for (auto& pname_pvalue : layout.int_parameters)
        {
            glTextureParameteri(m_name, pname_pvalue.first, pname_pvalue.second);
        }

        for (auto& pname_pvalue : layout.float_parameters)
        {
            glTextureParameterf(m_name, pname_pvalue.first, pname_pvalue.second);
        }

        if (generateMipmap && !customLevels)
        {
            m_levels = 1 + static_cast<GLsizei>(std::floor(std::log2(std::max(m_depth, std::max(m_width, m_height)))));
        }

        glTextureStorage3D(m_name, 1, m_internal_format, m_width, m_height, m_depth);

        if (data != nullptr)
        {
            glTextureSubImage3D(m_name, 0, 0, 0, 0, m_width, m_height, m_depth, m_format, m_type, data);
        }

        if (generateMipmap)
        {
            glGenerateTextureMipmap(m_name);
        }

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture3D::reload - texture id: " + m_id + " - OpenGL error " +
                                   std::to_string(err));
        }
    }

    inline TextureLayout Texture3D::getTextureLayout() const
    {
        return TextureLayout(m_internal_format, m_width, m_height, m_depth, m_format, m_type, m_levels);
    }

    inline unsigned int Texture3D::getWidth()
    {
        return m_width;
    }

    inline unsigned int Texture3D::getHeight()
    {
        return m_height;
    }

    inline unsigned int Texture3D::getDepth()
    {
        return m_depth;
    }

} // namespace glowl

#endif // GLOWL_TEXTURE3D_HPP
