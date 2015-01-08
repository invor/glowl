#ifndef texture_h
#define texture_h

#include <GL/glew.h>
#include <iostream>

class Texture
{
protected:
	/*	
	/	Integer ids seemed like a bad idea for loading texture files,
	/	that themselves -unlike custom material files- won't contain such an id.
	*/
	std::string m_name;

	GLuint m_handle;

	GLenum m_internal_format;
	GLenum m_format;
	GLenum m_type;
public:
	Texture(std::string name, GLint internal_format, GLenum format, GLenum type)
		: m_name(name), m_internal_format(internal_format), m_format(format), m_type(type) {}
	~Texture() {glDeleteTextures(1,&m_handle);}

	virtual void bindTexture() const = 0;
	virtual void texParameteri(GLenum, GLenum) = 0;

	std::string getName() {return m_name;}
	GLuint getHandle() {return m_handle;}
};

#endif
