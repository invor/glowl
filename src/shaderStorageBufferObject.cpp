#include "shaderStorageBufferObject.h"

ShaderStorageBufferObject::ShaderStorageBufferObject() : m_handle(0), m_index(-1), m_size(0), m_written_size(0), m_reset_data(0)
{
}

ShaderStorageBufferObject::~ShaderStorageBufferObject()
{
}

bool ShaderStorageBufferObject::create(unsigned int size, GLuint index, const GLvoid * data)
{
	/*	only generate a new buffer once */
	if(m_handle == 0)
	{
		glGenBuffers(1, &m_handle);
	}

	if(m_size != size)
	{
		/*	reallocate reset buffer */
		if(m_reset_data != 0)
			delete m_reset_data;
		
		m_reset_data = new GLint[size];
		for(unsigned int i=0; i<size; i++)
			m_reset_data[i] = -1;

		/*	if this function was called in order to change buffer size, unbind buffer from it's previous binding point */
		unbind();

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);

		m_size = size;

		//std::cout<<"I was called! :)"<<std::endl;
	}

	return true;
}

void ShaderStorageBufferObject::reset()
{
	/*	only reset an existing buffer */
	if(m_handle != 0)
	{
		/*	if this function was called in order to change buffer size, unbind buffer from it's previous binding point */
		unbind();

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_size, m_reset_data, GL_DYNAMIC_DRAW);
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
	m_index = index;
}

void ShaderStorageBufferObject::unbind()
{
	if(m_index != -1)
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, 0);
}

GLuint ShaderStorageBufferObject::getSize()
{
	return m_size;
}
