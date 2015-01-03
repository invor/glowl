#ifndef mesh_h
#define mesh_h

/*	Include system libraries */
#include <string>
#include <GL/glew.h>
#include <iostream>

class Mesh
{
private:
	const std::string m_name;
	GLuint m_num_vertices;
	GLenum m_mesh_type;

	/*	Handle of the vertex array on the GPU */
	GLuint m_va_handle;
	/*	Handle of the vertex buffer object on the GPU */
	GLuint m_vbo_handle;
	/*	Handle of the index buffer object  on the GPU */
	GLuint m_ibo_handle;

public:
	Mesh();
	~Mesh();

	Mesh(const std::string name);

	bool bufferDataFromArray(const GLvoid* vertex_data, const GLuint *index_data, const GLsizei va_size, const GLsizei vi_size, GLenum mesh_type);

	void draw(int num_instances = 1);

	void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	void setVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
	void setVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);

	const std::string getName() {return m_name;}

private:
	/* Private copy constructor. No going around deleting copies of OpenGL Object with identical handles! */
	Mesh(Mesh &cpy) {}
};

#endif
