/*
 * Texture2DArray.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_TEXTURE2DARRAY_HPP
#define GLOWL_TEXTURE2DARRAY_HPP

#include <algorithm>
#include <cmath>
#include <iostream>

#include "Texture.hpp"

namespace glowl
{

    /**
     * \class Texture2DArray
     *
     * \brief Encapsulates 2D texture array functionality.
     *
     * \author Michael Becher
     */
    class Texture2DArray : public Texture
    {
    public:
        /**
         * \brief Texture2DArray constructor.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        Texture2DArray(std::string id, TextureLayout const& layout, GLvoid* data, bool generateMipmap = false);
        Texture2DArray(const Texture2DArray&) =
            delete; // TODO: think of meaningful copy operation...maybe copy texture content to new texture object?
        Texture2DArray(Texture2DArray&& other) = delete;
        Texture2DArray& operator=(const Texture2DArray& rhs) = delete;
        Texture2DArray& operator=(Texture2DArray&& rhs) = delete;
        ~Texture2DArray();

        void bindTexture() const;

        void updateMipmaps();

        TextureLayout getTextureLayout() const;

        unsigned int getWidth() const;
        unsigned int getHeigth() const;
        unsigned int getLayers() const;

    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_layers;
    };

    inline Texture2DArray::Texture2DArray(std::string          id,
                                          TextureLayout const& layout,
                                          GLvoid*              data,
                                          bool                 generateMipmap)
        : Texture(id, layout.internal_format, layout.format, layout.type, layout.levels),
          m_width(layout.width),
          m_height(layout.height),
          m_layers(layout.depth)
    {
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_name);

        for (auto& pname_pvalue : layout.int_parameters)
        {
            glTextureParameteri(m_name, pname_pvalue.first, pname_pvalue.second);
        }

        for (auto& pname_pvalue : layout.float_parameters)
        {
            glTextureParameterf(m_name, pname_pvalue.first, pname_pvalue.second);
        }

        GLsizei levels = 1;

        if (generateMipmap)
        {
            levels = std::min(layout.levels, 1 + static_cast<GLsizei>(std::floor(std::log2(std::max(m_width, m_height)))));
        }

        glTextureStorage3D(m_name, levels, m_internal_format, m_width, m_height, m_layers);

        if (data != nullptr)
        {
            glTextureSubImage3D(m_name, 0, 0, 0, 0, m_width, m_height, m_layers, m_format, m_type, data);
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
            // "Do something cop!"
            std::cerr << "GL error during array texture (id: " << id << ") creation: " << err << std::endl;
        }
    }

    inline Texture2DArray::~Texture2DArray() {
        glDeleteTextures(1, &m_name);
    }

    inline void Texture2DArray::bindTexture() const
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_name);
    }

    inline void Texture2DArray::updateMipmaps()
    {
        glGenerateTextureMipmap(m_name);
    }

    inline TextureLayout Texture2DArray::getTextureLayout() const
    {
        return TextureLayout(m_internal_format, m_width, m_height, m_layers, m_format, m_type, m_levels);
    }

    inline unsigned int Texture2DArray::getWidth() const
    {
        return m_width;
    }

    inline unsigned int Texture2DArray::getHeigth() const
    {
        return m_height;
    }

    inline unsigned int Texture2DArray::getLayers() const
    {
        return m_layers;
    }

} // namespace glowl

#endif // GLOWL_TEXTURE2DARRAY_HPP
