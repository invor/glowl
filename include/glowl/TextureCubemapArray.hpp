/*
 * TextureCubemapArray.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef TextureCubemapArray_hpp
#define TextureCubemapArray_hpp

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
        TextureCubemapArray(std::string id,
            GLint internal_format,
            unsigned int width,
            unsigned int height,
            unsigned int layers,
            GLenum format,
            GLenum type,
            GLsizei levels,
            GLvoid* data,
            bool generateMipmap = false);
        TextureCubemapArray(const TextureCubemapArray&) = delete; // TODO: think of meaningful copy operation...maybe copy texture context to new texture object?
        TextureCubemapArray(TextureCubemapArray&& other) = delete;
        TextureCubemapArray& operator=(const TextureCubemapArray& rhs) = delete;
        TextureCubemapArray& operator=(TextureCubemapArray&& rhs) = delete;

        /**
        * \brief Reload the texture with a new size but unchanged format and type.
        * \param width Specifies the new width of the texture in pixels.
        * \param height Specifies the new height of the texture in pixels.
        * \param layers Specifies the new number of layers in the texture array.
        * \param data Pointer to the new texture data.
        * \return Returns true if the texture was succesfully created, false otherwise
        */
        bool reload(unsigned int width, unsigned int height, unsigned int layers, GLvoid * data, bool generateMipmap = false);

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

    inline TextureCubemapArray::TextureCubemapArray(std::string id,
        GLint internal_format,
        unsigned int width,
        unsigned int height,
        unsigned int layers,
        GLenum format,
        GLenum type,
        GLsizei levels,
        GLvoid* data,
        bool generateMipmap)
        : Texture(id, internal_format, format, type, levels), m_width(width), m_height(height), m_layers(layers)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_name);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 1, m_internal_format, m_width, m_height, m_layers);

        if (data != nullptr)
            glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, 0, m_width, m_height, m_layers, m_format, m_type, data);

        if (generateMipmap)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);

        if (glGetError() == GL_NO_ERROR)
        {
            // "Do something cop!"
        }
    }

    inline bool TextureCubemapArray::reload(unsigned int width, unsigned int height, unsigned int layers, GLvoid * data, bool generateMipmap)
    {
        m_width = width;
        m_height = height;
        m_layers = layers;

        glDeleteTextures(1, &m_name);

        glGenTextures(1, &m_name);
        assert(m_name > 0);

        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_name);

        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 1, m_internal_format, m_width, m_height, m_layers);

        if (data != nullptr)
            glTexSubImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, 0, 0, 0, m_width, m_height, m_layers, m_format, m_type, data);

        if (generateMipmap)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);

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
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_name);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
    }

    inline void TextureCubemapArray::texParameteri(GLenum pname, GLenum param)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_name);
        glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, pname, param);
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
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

}

#endif // !TextureCubemapArray_hpp
