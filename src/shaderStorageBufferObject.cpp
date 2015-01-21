#include "shaderStorageBufferObject.h"

ShaderStorageBufferObject::ShaderStorageBufferObject(unsigned int size, const GLvoid * data)
	: m_handle(0), m_size(size), m_reset_data(size,-1), m_written_size(0)
{
	/* make clang++ compiler 'unused variable' warning go away */
	if(0 && m_written_size)
	{
	}

	glGenBuffers(1, &m_handle);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_handle);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

ShaderStorageBufferObject::~ShaderStorageBufferObject()
{
	glDeleteBuffers(1,&m_handle);
}

bool ShaderStorageBufferObject::reload(unsigned int size, GLuint index, const GLvoid * data)
{
	m_size = size;

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_handle);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);

	/* TODO: check for errors and maybe don't return true? */
	return true;
}

void ShaderStorageBufferObject::reset()
{
	/*	only reset an existing buffer */
	if(m_handle != 0)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_size, m_reset_data.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	}
}

bool ShaderStorageBufferObject::map(void *& memory_ptr)
{
	memory_ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	if(memory_ptr == NULL)
		return false;
	else
		return true;
}

void ShaderStorageBufferObject::unmap()
{
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void ShaderStorageBufferObject::bind(GLuint index)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_handle);
}

GLuint ShaderStorageBufferObject::getSize()
{
	return m_size;
}
