/*
 * Texture3Dview.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_TEXTURE3DVIEW_HPP
#define GLOWL_TEXTURE3DVIEW_HPP

#include "Exceptions.hpp"
#include "Texture.hpp"
#include "Texture3D.hpp"

namespace glowl
{

    /**
     * \class TextureCubemapArray
     *
     * \brief Encapsulates 3D texture view functionality.
     *
     * \author Michael Becher
     */
    class Texture3DView : public Texture
    {
    public:
        /**
         * \brief Texture3DView constructor.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        Texture3DView(std::string          id,
                      Texture3D const&     source_texture,
                      TextureLayout const& layout,
                      GLuint               minlevel,
                      GLuint               numlevels,
                      GLuint               minlayer,
                      GLuint               numlayers);
        ~Texture3DView();

        void bindTexture() const;

        TextureLayout getTextureLayout() const;

        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getDepth();

    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_depth;
    };

    inline Texture3DView::Texture3DView(std::string          id,
                                        Texture3D const&     source_texture,
                                        TextureLayout const& layout,
                                        GLuint               minlevel,
                                        GLuint               numlevels,
                                        GLuint               minlayer,
                                        GLuint               numlayers)
        : Texture(id, layout.internal_format, layout.format, layout.type, layout.levels)
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

        glTextureView(m_name,
                      GL_TEXTURE_3D,
                      source_texture.getName(),
                      m_internal_format,
                      minlevel,
                      numlevels,
                      minlayer,
                      numlayers);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            throw TextureException("Texture3DView::Texture3DView - texture id: " + m_id + " - OpenGL error " +
                                     std::to_string(err));
        }
    }

    inline Texture3DView::~Texture3DView()
    {
        glDeleteTextures(1, &m_name);
    }

    inline void Texture3DView::bindTexture() const
    {
        glBindTexture(GL_TEXTURE_3D, m_name);
    }

    inline TextureLayout Texture3DView::getTextureLayout() const
    {
        return TextureLayout(m_internal_format, m_width, m_height, m_depth, m_format, m_type, m_levels);
    }

    inline unsigned int Texture3DView::getWidth()
    {
        return m_width;
    }

    inline unsigned int Texture3DView::getHeight()
    {
        return m_height;
    }

    inline unsigned int Texture3DView::getDepth()
    {
        return m_depth;
    }

} // namespace glowl

#endif // GLOWL_TEXTURE3DVIEW_HPP
