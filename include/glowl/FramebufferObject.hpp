/*
 * FramebufferObject.hpp
 * 
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef FramebufferObject_hpp
#define FramebufferObject_hpp

/*	Include space-lion files */
#include "Texture2D.hpp"

/*	Include system libraries */
#include <vector>
//#include <iostream>
#include <string>
#include <memory>

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
private:
	/** Handle of the FBO */
	GLuint m_handle;
	/** Colorbuffers attached to the FBO */
	std::vector<std::shared_ptr<Texture2D>> m_colorbuffers;
	/** Handle of the depthbuffer */
	GLuint m_depthbuffer;
	/** Handle of the stencilbuffer */
	GLuint m_stencilbuffer;

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
    /**
    * \brief FramebufferObject constructor.
    *
    * Note: Active OpenGL context required for construction.
    * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
    */
	FramebufferObject(int width, int height, bool has_depth = false, bool has_stencil = false);

    /**
    * \brief FramebufferObject constructor.
    *
    * Note: Active OpenGL context required for construction.
    * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
    */
    FramebufferObject(std::string const& debug_label, int width, int height, bool has_depth = false, bool has_stencil = false);

	~FramebufferObject();

	/*	Deleted copy constructor (C++11). Don't wanna go around copying objects with OpenGL handles. */
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
	bool createColorAttachment(GLenum internalFormat, GLenum format, GLenum type);

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
	bool checkStatus() const;

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
	int getWidth() const {return m_width;}

	/**
	* \brief Get the height of the framebuffer object's color attachments
	* \return Returns height.
	*/
	int getHeight() const {return m_height;}

	const std::string& getLog() const {return m_log;}
};

inline FramebufferObject::FramebufferObject(int width, int height, bool has_depth, bool has_stencil) : m_width(width), m_height(height)
{
    glGenFramebuffers(1, &m_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    if (has_depth)
    {
        glGenRenderbuffers(1, &m_depthbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    else
    {
        m_depthbuffer = 0;
    }

    /*
    /	TODO: stencilbuffer
    */
    if (0 && has_stencil)
    {
        has_stencil = !has_stencil;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline FramebufferObject::FramebufferObject(std::string const & debug_label, int width, int height, bool has_depth, bool has_stencil)
    : FramebufferObject(width,height,has_depth,has_stencil)
{
    m_debug_label = debug_label;
#if _DEBUG
    glObjectLabel(GL_FRAMEBUFFER, this->m_handle, static_cast<GLsizei>(m_debug_label.length()), m_debug_label.c_str());
#endif
}

inline FramebufferObject::~FramebufferObject()
{
    /*	Delete framebuffer resources. Texture delete themselves when the vector is destroyed. */
    if (m_depthbuffer != 0) glDeleteRenderbuffers(1, &m_depthbuffer);

    /*	Delete framebuffer object */
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    glDeleteFramebuffers(1, &m_handle);
}

inline bool FramebufferObject::createColorAttachment(GLenum internalFormat, GLenum format, GLenum type)
{
    GLint maxAttachments;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttachments);

    if (m_colorbuffers.size() == (GLuint)maxAttachments)
    {
        m_log.append("Maximum amount of color attachments reached.\n");
        return false;
    }

    unsigned int bufsSize = static_cast<unsigned int>(m_colorbuffers.size());

    TextureLayout color_attach_layout(internalFormat, m_width, m_height, 1, format, type, 1,
        { { GL_TEXTURE_MIN_FILTER ,GL_NEAREST },
            { GL_TEXTURE_MAG_FILTER ,GL_NEAREST },
            { GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE },
            { GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE } },
        { {} });
    std::shared_ptr<Texture2D> new_color_attachment(new Texture2D("fbo_" + std::to_string(m_handle) + "_color_attachment_" + std::to_string(bufsSize), color_attach_layout, nullptr));
    m_colorbuffers.push_back(new_color_attachment);

    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + bufsSize, GL_TEXTURE_2D, m_colorbuffers.back()->getName(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_drawBufs.push_back(GL_COLOR_ATTACHMENT0 + bufsSize);

    return true;
}

inline void FramebufferObject::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    //	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //	{
    //		m_log.append("Tried to use incomplete FBO. Fallback to default FBO.\n");
    //		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //	}
    //	else
    //{
    //	unsigned int bufsSize = static_cast<unsigned int>(m_colorbuffers.size());
    //	GLenum* drawBufs = new GLenum[bufsSize];
    //	for(GLuint i=0; i < bufsSize; i++)
    //	{
    //		drawBufs[i] = (GL_COLOR_ATTACHMENT0+i);
    //	}
    //	glDrawBuffers(bufsSize, drawBufs);
    //
    //	delete drawBufs;
    //}

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
    GLenum* drawBufs = new GLenum[bufsSize];
    for (GLuint i = 0; i < bufsSize; i++)
    {
        drawBufs[i] = (GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(bufsSize, drawBufs);
}

inline void FramebufferObject::bindColorbuffer(unsigned int index)
{
    if (index < m_colorbuffers.size()) m_colorbuffers[index]->bindTexture();
}

inline void FramebufferObject::bindDepthbuffer()
{
    glBindTexture(GL_TEXTURE_2D, m_depthbuffer);
}

inline void FramebufferObject::bindStencilbuffer()
{
    glBindTexture(GL_TEXTURE_2D, m_stencilbuffer);
}

inline bool FramebufferObject::checkStatus() const
{
    if (glCheckFramebufferStatus(m_handle) == GL_FRAMEBUFFER_COMPLETE) return true;
    return false;
}

inline void FramebufferObject::resize(int new_width, int new_height)
{
    m_width = new_width;
    m_height = new_height;

    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    GLenum attachment_point = GL_COLOR_ATTACHMENT0;

    for (auto& colorbuffer : m_colorbuffers)
    {
        //TODO add more convienient method
        TextureLayout color_attach_layout(colorbuffer->getInternalFormat(), m_width, m_height, 1, colorbuffer->getFormat(), colorbuffer->getType(), 1,
            { { GL_TEXTURE_MIN_FILTER ,GL_NEAREST },
                { GL_TEXTURE_MAG_FILTER ,GL_NEAREST },
                { GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE },
                { GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE } },
            {});

        colorbuffer->reload(color_attach_layout, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_point++, GL_TEXTURE_2D, colorbuffer->getName(), 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //resize depth buffer
    if (m_depthbuffer != 0)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, new_width, new_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

}

#endif // !FramebufferObject_hpp
