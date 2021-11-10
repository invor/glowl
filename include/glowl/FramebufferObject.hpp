/*
 * FramebufferObject.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_FRAMEBUFFEROBJECT_HPP
#define GLOWL_FRAMEBUFFEROBJECT_HPP

/* Include system libraries */
#include <memory>
#include <string>
#include <vector>

/* Include glowl files */
#include "Exceptions.hpp"
#include "Texture2D.hpp"
#include "glinclude.h"

namespace glowl
{

    /**
     * \class FramebufferObject
     *
     * \brief Encapsulates basic framebuffer object functionality.
     *
     * This class encapsulates basic framebuffer object functionality including creation of an FBO,
     * creation and adding of several color attachments and use it for rendering.
     *
     * \author Michael Becher
     */
    class FramebufferObject
    {
    public:
        struct ColorAttachmentSemantic
        {
            enum class ChannelSemantic : uint32_t
            {
                Unused,
                Unknown,
                Color_R,
                Color_G,
                Color_B,
                Normal_X,
                Normal_Y,
                Normal_Z,
                Depth,
                Roughness,
                Metalness,
                AmbientOcclusion
                // Add all possible things..
            };

            ChannelSemantic r = ChannelSemantic::Unknown;
            ChannelSemantic g = ChannelSemantic::Unknown;
            ChannelSemantic b = ChannelSemantic::Unknown;
            ChannelSemantic a = ChannelSemantic::Unknown;
        };

    private:
        /** Handle of the FBO */
        GLuint m_handle;
        /** Colorbuffers attached to the FBO */
        std::vector<std::pair<std::shared_ptr<Texture2D>, ColorAttachmentSemantic>> m_colorbuffers;

        /** Optional depth (and stencil) buffer texture */
        std::shared_ptr<Texture2D> m_depth_stencil;

        // TODO additional Texture2DView for read access of stencil buffer

        /** Width of the framebuffer i.e. it's color attachments */
        int m_width;
        /** Height of the framebuffer i.e. it's color attachments */
        int m_height;

        /** List of all draw buffer targets (i.e. all color attachments) */
        std::vector<GLenum> m_drawBufs;

        /** Optional debug label, used as glObjectLabel in DEBUG */
        std::string m_debug_label;

        std::string m_log;

    public:
        enum DepthStencilType
        {
            NONE,
            DEPTH16,
            DEPTH24,
            DEPTH32F,
            DEPTH24_STENCIL8,
            DEPTH32F_STENCIL8
        };

        /**
         * \brief FramebufferObject constructor.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        FramebufferObject(int width, int height, DepthStencilType depth_stencil_type = DEPTH24);

        /**
         * \brief FramebufferObject constructor.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        FramebufferObject(std::string const& debug_label,
                          int                width,
                          int                height,
                          DepthStencilType   depth_stencil_type = DEPTH24);

        ~FramebufferObject();

        /* Deleted copy constructor (C++11). Don't wanna go around copying objects with OpenGL handles. */
        FramebufferObject(const FramebufferObject& cpy) = delete;

        FramebufferObject(FramebufferObject&& other) = delete;

        FramebufferObject& operator=(const FramebufferObject& rhs) = delete;

        FramebufferObject& operator=(FramebufferObject&& rhs) = delete;

        /**
        * \brief Adds one color attachment to the framebuffer.
        * \note New colorbuffers are added at the end of the colorbuffer vector.
                The index i within the storage vector and the used attachment point
                GL_COLOR_ATTACHMENTi will be the same.
        * \param internalFormat Specifies channels+size (e.g. GL_R32F)
        * \param format Specifies channels (e.g. GL_RED)
        * \param type Specifies datatype (e.g. GL_FLOAT)
        * \return Returns true if a color attachment was added, false otherwise
        */
        void createColorAttachment(GLenum                  internalFormat,
                                   GLenum                  format,
                                   GLenum                  type,
                                   ColorAttachmentSemantic semantic = ColorAttachmentSemantic());

        std::shared_ptr<Texture2D> getColorAttachment(unsigned int index) const;

        std::shared_ptr<Texture2D> getDepthStencil() const;

        /**
         * \brief Bind this framebuffer object with all its color attachments
         */
        void bind();

        /**
         * \brief Bind this framebuffer object with a given set of draw buffers
         */
        void bind(const std::vector<GLenum>& draw_buffers);
        void bind(std::vector<GLenum>&& draw_buffers);

        /**
         * \brief Bind the framebuffer to GL_READ_FRAMEBUFFER
         * \param index Set glReadBuffer to color attachment #index or 0, if index > #color attachments
         */
        void bindToRead(unsigned int index);

        /**
         * \brief Bind the framebuffer to GL_DRAW_FRAMEBUFFER using all color attachments for glDrawBuffers
         */
        void bindToDraw();

        /**
         * \brief Bind a color attachment as GL_TEXTURE_2D.
         * \param index Specifies which color is bound. If index > #attachments, the method simply won't bind a texture.
         */
        void bindColorbuffer(unsigned int index);

        /**
         * \brief Bind the depth buffer as GL_TEXTURE_2D.
         * \note Work in progress. Use of this method is discouraged.
         */
        void bindDepthbuffer();

        /**
         * \brief Bind the stencil buffer as GL_TEXTURE_2D.
         * \note Work in progress. Use of this method is higly discouraged.
         */
        void bindStencilbuffer();

        /**
         * \brief Check the framebuffer object for completeness.
         * \return Returns true if the framebuffer object is complete, false otherwise.
         */
        GLenum checkStatus(GLenum target) const;

        /**
         * \brief Resize the framebuffer object, i.e. it's color attachments.
         * \note Might be a bit costly to use often.
         * \param new_width Specifies the new framebuffer width.
         * \param new_width Specifies the new framebuffer height.
         */
        void resize(int new_width, int new_height);

        /**
         * \brief Get the width of the framebuffer object's color attachments
         * \return Returns widths.
         */
        int getWidth() const
        {
            return m_width;
        }

        /**
         * \brief Get the height of the framebuffer object's color attachments
         * \return Returns height.
         */
        int getHeight() const
        {
            return m_height;
        }

        std::string const& getLog() const
        {
            return m_log;
        }
    };

    inline FramebufferObject::FramebufferObject(int width, int height, DepthStencilType depth_stencil_type)
        : m_width(width),
          m_height(height)
    {
        glCreateFramebuffers(1, &m_handle);

        if (depth_stencil_type != FramebufferObject::DepthStencilType::NONE)
        {
            GLint  internal_format;
            GLenum format = GL_DEPTH_COMPONENT;
            GLenum type;

            switch (depth_stencil_type)
            {
            case glowl::FramebufferObject::DEPTH16:
                internal_format = GL_DEPTH_COMPONENT16;
                type = GL_FLOAT;
                break;
            case glowl::FramebufferObject::DEPTH24:
                internal_format = GL_DEPTH_COMPONENT24;
                type = GL_FLOAT;
                break;
            case glowl::FramebufferObject::DEPTH32F:
                internal_format = GL_DEPTH_COMPONENT32F;
                type = GL_FLOAT;
                break;
            case glowl::FramebufferObject::DEPTH24_STENCIL8:
                internal_format = GL_DEPTH24_STENCIL8;
                type = GL_UNSIGNED_INT_24_8;
                break;
            case glowl::FramebufferObject::DEPTH32F_STENCIL8:
                internal_format = GL_DEPTH32F_STENCIL8;
                type = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
                break;
            default:
                break;
            }

            TextureLayout depth_stencil_layout(internal_format,
                                               m_width,
                                               m_height,
                                               1,
                                               format,
                                               type,
                                               1,
                                               {{GL_TEXTURE_MIN_FILTER, GL_NEAREST},
                                                {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
                                                {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
                                                {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE}},
                                               {});

            m_depth_stencil = std::make_shared<Texture2D>("", depth_stencil_layout, nullptr);

            if (depth_stencil_type == FramebufferObject::DepthStencilType::DEPTH24_STENCIL8 ||
                depth_stencil_type == FramebufferObject::DepthStencilType::DEPTH32F_STENCIL8)
            {
                glNamedFramebufferTexture(m_handle, GL_DEPTH_STENCIL_ATTACHMENT, m_depth_stencil->getName(), 0);
            }
            else
            {
                glNamedFramebufferTexture(m_handle, GL_DEPTH_ATTACHMENT, m_depth_stencil->getName(), 0);
            }
        }
    }

    inline FramebufferObject::FramebufferObject(std::string const& debug_label,
                                                int                width,
                                                int                height,
                                                DepthStencilType   depth_stencil_type)
        : FramebufferObject(width, height, depth_stencil_type)
    {
        m_debug_label = debug_label;
#if _DEBUG
        glObjectLabel(GL_FRAMEBUFFER,
                      this->m_handle,
                      static_cast<GLsizei>(m_debug_label.length()),
                      m_debug_label.c_str());
#endif
    }

    inline FramebufferObject::~FramebufferObject()
    {
        /* Delete framebuffer object */
        glDeleteFramebuffers(1, &m_handle);
    }

    inline void FramebufferObject::createColorAttachment(GLenum                  internalFormat,
                                                         GLenum                  format,
                                                         GLenum                  type,
                                                         ColorAttachmentSemantic semantic)
    {
        GLint maxAttachments;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttachments);

        if (m_colorbuffers.size() == (GLuint) maxAttachments)
        {
            m_log.append("Maximum amount of color attachments reached.\n");

            throw FramebufferObjectException("FramebufferObject::createColorAttachment - log:\n" + m_log);
        }

        unsigned int bufsSize = static_cast<unsigned int>(m_colorbuffers.size());

        TextureLayout color_attach_layout(internalFormat,
                                          m_width,
                                          m_height,
                                          1,
                                          format,
                                          type,
                                          1,
                                          {{GL_TEXTURE_MIN_FILTER, GL_NEAREST},
                                           {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
                                           {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
                                           {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE}},
                                          {});
        m_colorbuffers.push_back({std::make_shared<Texture2D>("fbo_" + std::to_string(m_handle) + "_color_attachment_" +
                                                                  std::to_string(bufsSize),
                                                              color_attach_layout,
                                                              nullptr),
                                  semantic});

        glNamedFramebufferTexture(m_handle,
                                  GL_COLOR_ATTACHMENT0 + bufsSize,
                                  std::get<0>(m_colorbuffers.back())->getName(),
                                  0);

        m_drawBufs.push_back(GL_COLOR_ATTACHMENT0 + bufsSize);
    }

    inline std::shared_ptr<Texture2D> FramebufferObject::getColorAttachment(unsigned int index) const
    {
        return index < m_colorbuffers.size() ? std::get<0>(m_colorbuffers[index]) : nullptr;
    }

    inline std::shared_ptr<Texture2D> FramebufferObject::getDepthStencil() const
    {
        return m_depth_stencil;
    }

    inline void FramebufferObject::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

        glDrawBuffers(static_cast<unsigned int>(m_drawBufs.size()), m_drawBufs.data());
    }

    inline void FramebufferObject::bind(const std::vector<GLenum>& draw_buffers)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

        glDrawBuffers(static_cast<unsigned int>(draw_buffers.size()), draw_buffers.data());
    }

    inline void FramebufferObject::bind(std::vector<GLenum>&& draw_buffers)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

        glDrawBuffers(static_cast<unsigned int>(draw_buffers.size()), draw_buffers.data());
    }

    inline void FramebufferObject::bindToRead(unsigned int index)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
        GLenum readBuffer;
        if (index < static_cast<unsigned int>(m_colorbuffers.size()))
            readBuffer = (GL_COLOR_ATTACHMENT0 + index);

        glReadBuffer(readBuffer);
    }

    inline void FramebufferObject::bindToDraw()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);
        unsigned int bufsSize = static_cast<unsigned int>(m_colorbuffers.size());
        GLenum*      drawBufs = new GLenum[bufsSize];
        for (GLuint i = 0; i < bufsSize; i++)
        {
            drawBufs[i] = (GL_COLOR_ATTACHMENT0 + i);
        }
        glDrawBuffers(bufsSize, drawBufs);
    }

    inline void FramebufferObject::bindColorbuffer(unsigned int index)
    {
        if (index < m_colorbuffers.size())
            std::get<0>(m_colorbuffers[index])->bindTexture();
    }

    inline void FramebufferObject::bindDepthbuffer()
    {
        m_depth_stencil->bindTexture();
    }

    inline GLenum FramebufferObject::checkStatus(GLenum target) const
    {
        return glCheckNamedFramebufferStatus(m_handle, GL_FRAMEBUFFER);
    }

    inline void FramebufferObject::resize(int new_width, int new_height)
    {
        m_width = new_width;
        m_height = new_height;

        GLenum attachment_point = GL_COLOR_ATTACHMENT0;

        for (auto& colorbuffer : m_colorbuffers)
        {
            // TODO add more convienient method
            TextureLayout color_attach_layout(std::get<0>(colorbuffer)->getInternalFormat(),
                                              m_width,
                                              m_height,
                                              1,
                                              std::get<0>(colorbuffer)->getFormat(),
                                              std::get<0>(colorbuffer)->getType(),
                                              1,
                                              {{GL_TEXTURE_MIN_FILTER, GL_NEAREST},
                                               {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
                                               {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
                                               {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE}},
                                              {});

            std::get<0>(colorbuffer)->reload(color_attach_layout, nullptr);

            glNamedFramebufferTexture(m_handle, attachment_point++, std::get<0>(colorbuffer)->getName(), 0);
        }

        // resize depth buffer
        if (m_depth_stencil != nullptr)
        {
            TextureLayout stencil_depth_layout(m_depth_stencil->getInternalFormat(),
                                               m_width,
                                               m_height,
                                               1,
                                               m_depth_stencil->getFormat(),
                                               m_depth_stencil->getType(),
                                               1,
                                               {{GL_TEXTURE_MIN_FILTER, GL_NEAREST},
                                                {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
                                                {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
                                                {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE}},
                                               {});

            m_depth_stencil->reload(stencil_depth_layout, nullptr);

            if (m_depth_stencil->getInternalFormat() == GL_DEPTH24_STENCIL8 ||
                m_depth_stencil->getInternalFormat() == GL_DEPTH32F_STENCIL8)
            {
                glNamedFramebufferTexture(m_handle, GL_DEPTH_STENCIL_ATTACHMENT, m_depth_stencil->getName(), 0);
            }
            else
            {
                glNamedFramebufferTexture(m_handle, GL_DEPTH_ATTACHMENT, m_depth_stencil->getName(), 0);
            }
        }
    }

} // namespace glowl

#endif // GLOWL_FRAMEBUFFEROBJECT_HPP
