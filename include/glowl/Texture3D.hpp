/*
 * Texture3D.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef Texture3D_hpp
#define Texture3D_hpp

#include <iostream>

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
        Texture3D(std::string id, TextureLayout const& layout, GLvoid* data);
        Texture3D(const Texture3D&) = delete;
        Texture3D(Texture3D&& other) = delete;
        Texture3D& operator=(const Texture3D& rhs) = delete;
        Texture3D& operator=(Texture3D&& rhs) = delete;

        /**
         * \brief Bind the texture.
         */
        void bindTexture() const;

        void updateMipmaps();

        /**
         * \brief Reload the texture.
         * \param data Pointer to the new texture data.
         */
        void reload(TextureLayout const& layout, GLvoid* data);

        TextureLayout getTextureLayout() const;

        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getDepth();

    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_depth;
    };

    inline Texture3D::Texture3D(std::string id, TextureLayout const& layout, GLvoid* data)
        : Texture(id, layout.internal_format, layout.format, layout.type, layout.levels),
          m_width(layout.width),
          m_height(layout.height),
          m_depth(layout.depth)
    {
        glBindTexture(GL_TEXTURE_3D, m_name);

        for (auto& pname_pvalue : layout.int_parameters)
        {
            glTexParameteri(GL_TEXTURE_3D, pname_pvalue.first, pname_pvalue.second);
        }

        for (auto& pname_pvalue : layout.float_parameters)
        {
            glTexParameterf(GL_TEXTURE_3D, pname_pvalue.first, pname_pvalue.second);
        }

        glTexStorage3D(GL_TEXTURE_3D, 1, m_internal_format, m_width, m_height, m_depth);

        if (data != nullptr)
        {
            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, m_width, m_height, m_depth, m_format, m_type, data);
        }

        glBindTexture(GL_TEXTURE_3D, 0);

#ifndef GLOWL_NO_ARB_BINDLESS_TEXTURE
        m_texture_handle = glGetTextureHandleARB(m_name);
#endif

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            // "Do something cop!"
            std::cerr << "GL error during 3D texture (id:" << id << ") creation: " << err << std::endl;
        }
    }

    inline void Texture3D::bindTexture() const
    {
        glBindTexture(GL_TEXTURE_3D, m_name);
    }

    inline void Texture3D::updateMipmaps()
    {
        glBindTexture(GL_TEXTURE_3D, m_name);
        glGenerateMipmap(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, 0);
    }

    inline void Texture3D::reload(TextureLayout const& layout, GLvoid* data)
    {
        m_width = layout.width;
        m_height = layout.height;
        m_depth = layout.depth;
        m_internal_format = layout.internal_format;
        m_format = layout.format;
        m_type = layout.type;

        glDeleteTextures(1, &m_name);

        glGenTextures(1, &m_name);

        glBindTexture(GL_TEXTURE_3D, m_name);

        for (auto& pname_pvalue : layout.int_parameters)
            glTexParameteri(GL_TEXTURE_3D, pname_pvalue.first, pname_pvalue.second);

        for (auto& pname_pvalue : layout.float_parameters)
            glTexParameterf(GL_TEXTURE_3D, pname_pvalue.first, pname_pvalue.second);

        glTexStorage3D(GL_TEXTURE_3D, 1, m_internal_format, m_width, m_height, m_depth);

        if (data != nullptr)
            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, m_width, m_height, m_depth, m_format, m_type, data);

        glBindTexture(GL_TEXTURE_3D, 0);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            // "Do something cop!"
            std::cerr << "GL error during texture reloading: " << err << std::endl;
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

#endif // !Texture3D_hpp
