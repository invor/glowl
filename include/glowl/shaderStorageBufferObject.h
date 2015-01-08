#ifndef fetol_shaderStorageBufferObject_h
#define fetol_shaderStorageBufferObject_h

#include <GL/glew.h>

/*	std includes */
#include <iostream>
#include <vector>

class ShaderStorageBufferObject
{
public:
	ShaderStorageBufferObject(unsigned int size, const GLvoid * data);
	~ShaderStorageBufferObject();

	ShaderStorageBufferObject(ShaderStorageBufferObject& cpy) = delete;

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