#include "texture3D.h"

#include <cassert>

Texture3D::Texture3D(std::string name, GLint internal_format, unsigned int width, unsigned int height, unsigned int depth, GLenum format, GLenum type, GLvoid * data)
	: Texture(name,internal_format,format,type), m_width(width), m_height(height), m_depth(depth)
{
	glGenTextures(1, &m_handle);
	assert(m_handle > 0);

	glBindTexture(GL_TEXTURE_3D, m_handle);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, internal_format, m_width, m_height, m_depth, 0, format, type, data);
	glBindTexture(GL_TEXTURE_3D,0);

	if (glGetError() == GL_NO_ERROR)
	{
		// "Do something cop!"
	}
}

void Texture3D::bindTexture() const
{
	glBindTexture(GL_TEXTURE_3D, m_handle);
}

void Texture3D::bindImage(GLuint location, GLenum access) const
{
	glBindImageTexture(location, m_handle, 0, GL_TRUE, 0, access, m_internal_format);
}

void Texture3D::texParameteri(GLenum pname, GLenum param)
{
	glBindTexture(GL_TEXTURE_3D, m_handle);
	glTexParameteri(GL_TEXTURE_3D, pname, param);
	glBindTexture(GL_TEXTURE_3D,0);
}

void Texture3D::reload(unsigned int width, unsigned int height, unsigned int depth, GLvoid * data)
{
	m_width = width;
	m_height = height;
	m_depth = depth;

	glBindTexture(GL_TEXTURE_3D, m_handle);
	glTexImage3D(GL_TEXTURE_3D, 0, m_internal_format, m_width, m_height, m_depth, 0, m_format, m_type, data);
	glBindTexture(GL_TEXTURE_3D,0);
}

void Texture3D::reload(GLenum internal_format, unsigned int width, unsigned int height, unsigned int depth, GLenum format, GLenum type, GLvoid * data)
{
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_internal_format = internal_format;
	m_format = format;
	m_type = type;

	glBindTexture(GL_TEXTURE_3D, m_handle);
	glTexImage3D(GL_TEXTURE_3D, 0, internal_format, width, height, depth, 0, format, type, data);
	glBindTexture(GL_TEXTURE_3D, 0);
}

unsigned int Texture3D::getWidth()
{
	return m_width;
}

unsigned int Texture3D::getHeight()
{
	return m_height;
}

unsigned int Texture3D::getDepth()
{
	return m_depth;
}