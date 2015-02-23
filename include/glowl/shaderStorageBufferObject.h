#ifndef fetol_shaderStorageBufferObject_h
#define fetol_shaderStorageBufferObject_h

#include <GL/glew.h>

/*	std includes */
//#include <iostream>
#include <vector>

class ShaderStorageBufferObject
{
public:
	ShaderStorageBufferObject(unsigned int size, const GLvoid * data);

	template<typename Container>
	ShaderStorageBufferObject(const Container &datastorage)
		: m_handle(0), m_size(datastorage.size() * sizeof(Container::value_type)), m_reset_data(m_size,-1), m_written_size(0)
	{
		/* make clang++ compiler 'unused variable' warning go away */
		if(0 && m_written_size)
		{
		}

		glGenBuffers(1, &m_handle);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_handle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_size, datastorage.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	}


	~ShaderStorageBufferObject();

	/* Deleted copy constructor (C++11). No going around deleting copies of OpenGL Object with identical handles! */
	ShaderStorageBufferObject(const ShaderStorageBufferObject& cpy) = delete;

	bool reload(unsigned int size, GLuint index, const GLvoid * data);
	void reset();

	bool map(void *& memory_ptr);
	void unmap();
	void bind(GLuint index);

	GLuint getSize();
private:
	/**	OpenGL handle/id of the buffer object */
	GLuint m_handle;

	/**	Overall size of the buffer */
	GLuint m_size;

	/** Buffer filled with reset data */
	std::vector<GLint> m_reset_data;

	/**
	/	Size of the data that has actually been written to the buffer.
	/	Note that this has to be set manually (usually from an atomic integer) after
	/	usage of the buffer!
	*/
	GLuint m_written_size;
};

#endif
