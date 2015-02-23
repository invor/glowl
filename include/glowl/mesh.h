#ifndef mesh_h
#define mesh_h

/*	Include system libraries */
#include <string>
#include <GL/glew.h>
//#include <iostream>

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

	/* Deleted copy constructor (C++11). No going around deleting copies of OpenGL Object with identical handles! */
	Mesh(const Mesh &cpy) = delete;

	Mesh(const std::string name);

	template<typename VertexContainer, typename IndexContainer>
	bool bufferDataFromArray(const VertexContainer &vertices, const IndexContainer &indices, GLenum mesh_type)
	{
		if(vertices.size() < 1 || indices.size() < 1)
			return false;

		auto va_size = sizeof(VertexContainer::value_type) * vertices.size();
		auto vi_size = sizeof(IndexContainer::value_type) * indices.size();

		if(m_va_handle == 0 || m_vbo_handle == 0 || m_ibo_handle == 0)
		{
			glGenVertexArrays(1, &m_va_handle);
			glGenBuffers(1, &m_vbo_handle);
			glGenBuffers(1, &m_ibo_handle);
		}

		glBindVertexArray(m_va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle);
		glBufferData(GL_ARRAY_BUFFER, va_size, vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi_size, indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_num_vertices = (vi_size/sizeof(GLuint));
		m_mesh_type = mesh_type;

		return true;
	}

	bool bufferDataFromArray(const GLvoid* vertex_data, const GLuint* index_data, const GLsizei va_size, const GLsizei vi_size, GLenum mesh_type);

	void draw(int num_instances = 1);

	void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	void setVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
	void setVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);

	const std::string getName() {return m_name;}

};

#endif
