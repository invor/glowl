#ifndef fetol_shaderStorageBufferObject_h
#define fetol_shaderStorageBufferObject_h

#include <glew.h>

/*	std includes */
#include <iostream>

class ShaderStorageBufferObject
{
public:
	ShaderStorageBufferObject();
	~ShaderStorageBufferObject();

	bool create(unsigned int size, GLuint index, const GLvoid * data);
	void reset();

	bool map(void *& memory_ptr);
	void unmap();
	void bind(GLuint index);
	void unbind();

	GLuint getSize();
private:
	/**	OpenGL handle/id of the buffer object */
	GLuint m_handle;

	/** Index of the bind point if currently bound, -1 otherwise */
	GLuint m_index;

	/**	Overall size of the buffer */
	GLuint m_size;

	/** Buffer filled with reset data */
	GLint* m_reset_data;

	/**
	/	Size of the data that has actually been written to the buffer.
	/	Note that this has to be set manually (usually from an atomic integer) after
	/	usage of the buffer!
	*/
	GLuint m_written_size;
};

#endif