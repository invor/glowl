/*
 * Texture2D.hpp
 *
 * MIT License
 * Copyright (c) 2021 Michael Becher
 */

#ifndef GLOWL_TEXTURE2D_HPP
#define GLOWL_TEXTURE2D_HPP

#include <algorithm>
#include <cmath>

#include "Exceptions.hpp"
#include "Texture.hpp"

namespace glowl
{

    /**
     * \class Texture2D
     *
     * \brief Encapsulates 2D texture functionality.
     *
     * \author Michael Becher
     */
    class Texture2D : public Texture
    {
    public:
        /**
         * \brief Constructor that creates and loads a 2D texture.
         *
         * \param id A identifier given to the texture object
         * \param layout A TextureLayout struct that specifies size, format and parameters for the texture
         * \param data Pointer to the actual texture data.
         * \param generateMipmap Specifies whether a mipmap will be created for the texture
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        Texture2D(std::string id, TextureLayout const& layout, GLvoid const* data, bool generateMipmap = false);
        Texture2D(const Texture2D&) = delete;
        Texture2D(Texture2D&& other) = delete;
        Texture2D& operator=(const Texture2D& rhs) = delete;
        Texture2D& operator=(Texture2D&& rhs) = delete;
        ~Texture2D();

        /**
         * \brief Bind the texture.
         */
        void bindTexture() const;

        /**
        * \brief Generate texture by glGenTextures
        */
        void genTexture();

        /**
         * \brief Deletes texture
         */
        void deleteTexture();

        void updateMipmaps();

        /**
         * \brief Reload the texture with any new format, type and size.
         *
         * \param layout A TextureLayout struct that specifies size, format and parameters for the texture
         * \param data Pointer to the actual texture data.
         * \param generateMipmap Specifies whether a mipmap will be created for the texture
         */
        void reload(TextureLayout const& layout, GLvoid const* data, bool generateMipmap = false, bool customLevels = false);

        TextureLayout getTextureLayout() const;

        unsigned int getWidth() const;

        unsigned int getHeight() const;

    private:
        unsigned int m_width;
        unsigned int m_height;
    };

    inline Texture2D::Texture2D(std::string id, TextureLayout const& layout, GLvoid const* data, bool generateMipmap)
        : Texture(id, layout.internal_format, layout.format, layout.type, layout.levels),
          m_width(layout.width),
          m_height(layout.height)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_name);

        for (auto& pname_pvalue : layout.int_parameters)
            glTextureParameteri(m_name, pname_pvalue.first, pname_pvalue.second);

        for (auto& pname_pvalue : layout.float_parameters)
            glTextureParameterf(m_name, pname_pvalue.first, pname_pvalue.second);

        GLsizei levels = 1;

        if (generateMipmap)
        {
            levels = 1 + static_cast<GLsizei>(std::floor(std::log2(std::max(m_width, m_height))));
        }

        glTextureStorage2D(m_name, levels, m_internal_format, m_width, m_height);

        if (data != nullptr)
        {
            glTextureSubImage2D(m_name, 0, 0, 0, m_width, m_height, m_format, m_type, data);
        }

        if (generateMipmap)
        {
            glGenerateTextureMipmap(m_name);
        }

#ifndef GLOWL_NO_ARB_BINDLESS_TEXTURE
        m_texture_handle = glGetTextureHandleARB(m_name);
#endif

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture2D::Texture2D - texture id: " + m_id + " - OpenGL error " + std::to_string(err));
        }
    }

    inline Texture2D::~Texture2D()
    {
        glDeleteTextures(1,&m_name);
    }

    inline void Texture2D::bindTexture() const
    {
        glBindTexture(GL_TEXTURE_2D, m_name);
    }

    inline void Texture2D::genTexture()
    {
        glGenTextures(1, &m_name);
    }

    inline void Texture2D::deleteTexture()
    {
        glDeleteTextures(1, &m_name);
    }

    inline void Texture2D::updateMipmaps()
    {
        glGenerateTextureMipmap(m_name);
    }

    inline void Texture2D::reload(TextureLayout const& layout, GLvoid const* data, bool generateMipmap, bool customLevels)
    {
        m_width = layout.width;
        m_height = layout.height;
        m_internal_format = layout.internal_format;
        m_format = layout.format;
        m_type = layout.type;
        m_levels = layout.levels;
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture2D::reload - texture id: " + m_id + " - OpenGL error " +
                                   std::to_string(err));
        }
        glDeleteTextures(1, &m_name);
        err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture2D::reload - texture id: " + m_id + " - OpenGL error " +
                                   std::to_string(err));
        }
        glCreateTextures(GL_TEXTURE_2D, 1, &m_name);
        err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture2D::reload - texture id: " + m_id + " - OpenGL error " +
                                   std::to_string(err));
        }
        for (auto& pname_pvalue : layout.int_parameters)
            glTextureParameteri(m_name, pname_pvalue.first, pname_pvalue.second);

        for (auto& pname_pvalue : layout.float_parameters)
            glTextureParameterf(m_name, pname_pvalue.first, pname_pvalue.second);

        if (generateMipmap)
        {
            if (!customLevels)
                m_levels = 1 + static_cast<GLsizei>(std::floor(std::log2(std::max(m_width, m_height))));
        }

        glTextureStorage2D(m_name, m_levels, m_internal_format, m_width, m_height);

        err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture2D::reload - texture id: " + m_id + " - OpenGL error " +
                                   std::to_string(err));
        }
        if (data != nullptr)
        {
            glTextureSubImage2D(m_name, 0, 0, 0, m_width, m_height, m_format, m_type, data);
        }

        if (generateMipmap)
        {
            glGenerateTextureMipmap(m_name);
        }

        err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture2D::reload - texture id: " + m_id + " - OpenGL error " +
                                   std::to_string(err));
        }
    }

    inline TextureLayout Texture2D::getTextureLayout() const
    {
        return TextureLayout(m_internal_format, m_width, m_height, 1, m_format, m_type, m_levels);
    }

    inline unsigned int Texture2D::getWidth() const
    {
        return m_width;
    }

    inline unsigned int Texture2D::getHeight() const
    {
        return m_height;
    }

} // namespace glowl

#endif // GLOWL_TEXTURE2D_HPP
