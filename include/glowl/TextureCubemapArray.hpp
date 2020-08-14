/*
 * TextureCubemapArray.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_TEXTURECUBEMAPARRAY_HPP
#define GLOWL_TEXTURECUBEMAPARRAY_HPP

#include <cassert>

#include "Texture.hpp"

namespace glowl
{

    /**
     * \class TextureCubemapArray
     *
     * \brief Encapsulates cubemap texture array functionality.
     *
     * \author Michael Becher
     */
    class TextureCubemapArray : public Texture
    {
    public:
        /**
         * \brief TextureCubemapArray constructor.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        TextureCubemapArray(std::string  id,
                            GLint        internal_format,
                            unsigned int width,
                            unsigned int height,
                            unsigned int layers,
                            GLenum       format,
                            GLenum       type,
                            GLsizei      levels,
                            GLvoid*      data,
                            bool         generateMipmap = false);
        TextureCubemapArray(const TextureCubemapArray&) =
            delete; // TODO: think of meaningful copy operation...maybe copy texture context to new texture object?
        TextureCubemapArray(TextureCubemapArray&& other) = delete;
        TextureCubemapArray& operator=(const TextureCubemapArray& rhs) = delete;
        TextureCubemapArray& operator=(TextureCubemapArray&& rhs) = delete;
        ~TextureCubemapArray();

        /**
         * \brief Reload the texture with a new size but unchanged format and type.
         * \param width Specifies the new width of the texture in pixels.
         * \param height Specifies the new height of the texture in pixels.
         * \param layers Specifies the new number of layers in the texture array.
         * \param data Pointer to the new texture data.
         * \return Returns true if the texture was succesfully created, false otherwise
         */
        bool reload(unsigned int width,
                    unsigned int height,
                    unsigned int layers,
                    GLvoid*      data,
                    bool         generateMipmap = false);

        void bindTexture() const;

        void updateMipmaps();

        void texParameteri(GLenum pname, GLenum param);

        TextureLayout getTextureLayout() const;

        unsigned int getWidth() const;
        unsigned int getHeigth() const;
        unsigned int getLayers() const;

    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_layers;
    };

    inline TextureCubemapArray::TextureCubemapArray(std::string  id,
                                                    GLint        internal_format,
                                                    unsigned int width,
                                                    unsigned int height,
                                                    unsigned int layers,
                                                    GLenum       format,
                                                    GLenum       type,
                                                    GLsizei      levels,
                                                    GLvoid*      data,
                                                    bool         generateMipmap)
        : Texture(id, internal_format, format, type, levels), m_width(width), m_height(height), m_layers(layers)
    {
        glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &m_name);

        glTextureParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_name, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_name, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_name, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTextureStorage3D(m_name, 1, m_internal_format, m_width, m_height, m_layers);

        if (data != nullptr) {
            glTextureSubImage3D(m_name, 0, 0, 0, 0, m_width, m_height, m_layers, m_format, m_type, data);
        }

        if (generateMipmap)
        {
            glTextureParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glGenerateTextureMipmap(m_name);
        }

        if (glGetError() == GL_NO_ERROR)
        {
            // "Do something cop!"
        }
    }

    inline TextureCubemapArray::~TextureCubemapArray()
    {
        glDeleteTextures(1, &m_name);
    }

    inline bool TextureCubemapArray::reload(unsigned int width,
                                            unsigned int height,
                                            unsigned int layers,
                                            GLvoid*      data,
                                            bool         generateMipmap)
    {
        m_width = width;
        m_height = height;
        m_layers = layers;

        glDeleteTextures(1, &m_name);

        glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &m_name);
        assert(m_name > 0);

        glTextureParameteri(m_name, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_name, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(m_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTextureStorage3D(m_name, 1, m_internal_format, m_width, m_height, m_layers);

        if (data != nullptr) {
            glTextureSubImage3D(m_name, 0, 0, 0, 0, m_width, m_height, m_layers, m_format, m_type, data);
        }

        if (generateMipmap)
        {
            glTextureParameteri(m_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glGenerateTextureMipmap(m_name);
        }

        if (glGetError() == GL_NO_ERROR)
        {
            // "Do something cop!"
            return true;
        }
        else
        {
            return false;
        }
    }

    inline void TextureCubemapArray::bindTexture() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_name);
    }

    inline void TextureCubemapArray::updateMipmaps()
    {
        glGenerateTextureMipmap(m_name);
    }

    inline void TextureCubemapArray::texParameteri(GLenum pname, GLenum param)
    {
        glTextureParameteri(m_name, pname, param);
    }

    inline TextureLayout TextureCubemapArray::getTextureLayout() const
    {
        return TextureLayout(m_internal_format, m_width, m_height, m_layers, m_format, m_type, m_levels);
    }

    inline unsigned int TextureCubemapArray::getWidth() const
    {
        return m_width;
    }

    inline unsigned int TextureCubemapArray::getHeigth() const
    {
        return m_height;
    }

    inline unsigned int TextureCubemapArray::getLayers() const
    {
        return m_layers;
    }

} // namespace glowl

#endif // GLOWL_TEXTURECUBEMAPARRAY_HPP
