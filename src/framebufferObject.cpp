#include "framebufferObject.h"

FramebufferObject::FramebufferObject(int width, int height, bool has_depth, bool has_stencil) : m_width(width), m_height(height)
{
	glGenFramebuffers(1, &m_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	if(has_depth)
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
	if(0 && has_stencil)
	{
		has_stencil = !has_stencil;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FramebufferObject::~FramebufferObject()
{
	/*	Delete framebuffer resources. Texture delete themselves when the vector is destroyed. */	
	if (m_depthbuffer != 0) glDeleteRenderbuffers(1, &m_depthbuffer);

	/*	Delete framebuffer object */
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glDeleteFramebuffers(1, &m_handle);
}

bool FramebufferObject::createColorAttachment(GLenum internalFormat, GLenum format, GLenum type)
{	
	GLint maxAttachments;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttachments);

	if(m_colorbuffers.size() == (GLuint) maxAttachments) 
	{
		m_log.append("Maximum amount of color attachments reached.\n");
		return false;
	}

	unsigned int bufsSize = static_cast<unsigned int>(m_colorbuffers.size());
	std::shared_ptr<Texture2D> new_color_attachment(new Texture2D("fbo_"+std::to_string(m_handle)+"_color_attachment_"+std::to_string(bufsSize), internalFormat, m_width, m_height, format, type, NULL));
	m_colorbuffers.push_back(new_color_attachment);

	m_colorbuffers.back()->bindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+bufsSize, GL_TEXTURE_2D, m_colorbuffers.back()->getHandle(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_drawBufs.push_back(GL_COLOR_ATTACHMENT0+bufsSize);

	return true;
}

void FramebufferObject::bind()
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

	glDrawBuffers( static_cast<unsigned int>(m_drawBufs.size()) , m_drawBufs.data());
}

void FramebufferObject::bind(std::vector<GLenum> draw_buffers)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	glDrawBuffers( static_cast<unsigned int>(draw_buffers.size()) , draw_buffers.data());
}

void FramebufferObject::bindToRead(unsigned int index)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER,m_handle);
	GLenum readBuffer;
	if(index < static_cast<unsigned int>(m_colorbuffers.size()))
		readBuffer = (GL_COLOR_ATTACHMENT0+index);

	glReadBuffer(readBuffer);
}

void FramebufferObject::bindToDraw()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);
	unsigned int bufsSize = static_cast<unsigned int>(m_colorbuffers.size());
	GLenum* drawBufs = new GLenum[bufsSize];
	for(GLuint i=0; i < bufsSize; i++)
	{
		drawBufs[i] = (GL_COLOR_ATTACHMENT0+i);
	}
	glDrawBuffers(bufsSize, drawBufs);
}

void FramebufferObject::bindColorbuffer(unsigned int index)
{
	if (index < m_colorbuffers.size()) m_colorbuffers[index]->bindTexture();
}

void FramebufferObject::bindDepthbuffer()
{
	glBindTexture(GL_TEXTURE_2D, m_depthbuffer);
}

void FramebufferObject::bindStencilbuffer()
{
	glBindTexture(GL_TEXTURE_2D, m_stencilbuffer);
}

bool FramebufferObject::checkStatus()
{
	if(glCheckFramebufferStatus(m_handle) == GL_FRAMEBUFFER_COMPLETE) return true;
	return false;
}

void FramebufferObject::resize(int new_width, int new_height)
{
	m_width = new_width;
	m_height = new_height;

	for(auto& colorbuffer : m_colorbuffers)
	{
		colorbuffer->reload(m_width, m_height, NULL);
	}
}
