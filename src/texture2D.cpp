#include "texture2D.h"

#include <cassert>

Texture2D::Texture2D(std::string name, GLint internal_format, unsigned int width, unsigned int height, GLenum format, GLenum type, GLvoid * data)
	: Texture(name,internal_format,format,type), m_width(width), m_height(height)
{
	glGenTextures(1, &m_handle);
	assert(m_handle > 0);

	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, type, data);
	glBindTexture(GL_TEXTURE_2D,0);

	if (glGetError() == GL_NO_ERROR)
	{
		// "Do something cop!"
	}
}

void Texture2D::bindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture2D::texParameteri(GLenum pname, GLenum param)
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexParameteri(GL_TEXTURE_2D, pname, param);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture2D::reload(unsigned int width, unsigned int height, GLvoid * data)
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, m_internal_format, width, height, 0, m_format, m_type, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glGetError() == GL_NO_ERROR) return true;
	else return false;
}

bool Texture2D::reload(GLenum internal_format, unsigned int width, unsigned int height, GLenum format, GLenum type, GLvoid * data)
{
	m_internal_format = internal_format;
	m_format = format;
	m_type = type;

	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glGetError() == GL_NO_ERROR) return true;
	else return false;
}

unsigned int Texture2D::getWidth()
{
	return m_width;
}

unsigned int Texture2D::getHeight()
{
	return m_height;
}