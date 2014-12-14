#ifndef texture2D_h
#define texture2D_h

#include "texture.h"


/**
* \class Texture2D
*
* \brief Encapsulates basic 2D texure functionality.
*
* This class encapsulates basic 2D functionality including creation of a 2D texture,
* as well as setting parameters and binding it.
*
* \author Michael Becher
*
* \date 19th September 2013
*/
class Texture2D : public Texture
{
public:
	Texture2D() {}
	Texture2D(std::string in_filename) : Texture(in_filename) {}

	/**
	* \brief Bind the texture.
	*/
	void bindTexture() const;

	/**
	* \brief Set a texture parameter.
	* \param pname Specifies the parameter to be set.
	* \param param Specifies the value that the parameter is set to.
	*/
	void texParameteri(GLenum pname, GLenum param);

	/**
	* \brief Create and load the texture.
	* \param internal_format Specifies the internal format of the texture (e.g. GL_RGBA32F)
	* \param dim_x Specifies the width of the texture in pixels.
	* \param dim_y Specifies the height of the texture in pixels.
	* \param format Specifies the format of the texture (e.g. GL_RGBA)
	* \param type Specifies the type of the texture (e.g. GL_FLOAT)
	* \param data Pointer to the actual texture data.
	* \return Returns true if the texture was succesfully created, false otherwise
	*/
	bool load(GLint internal_format, int dim_x, int dim_y, GLenum format, GLenum type, GLvoid * data);

	/**
	* \brief Reload the texture with a new size but unchanged format and type.
	* \note Will not work if load() hasn't been called on this object yet.
	* \param dim_x Specifies the new width of the texture in pixels.
	* \param dim_y Specifies the new height of the texture in pixels.
	* \param data Pointer to the new texture data.
	* \return Returns true if the texture was succesfully created, false otherwise
	*/
	bool reload(int dim_x, int dim_y, GLvoid * data);

	/**
	* \brief Reload the texture with any new format, type and size.
	* \note Will not work if load() hasn't been called on this object yet.
	* \param internal_format Specifies the new internal format of the texture (e.g. GL_RGBA)
	* \param dim_x Specifies the new width of the texture in pixels.
	* \param dim_y Specifies the new height of the texture in pixels.
	* \param format Specifies the new format of the texture (e.g. GL_RGBA32F)
	* \param type Specifies the new type of the texture (e.g. GL_FLOAT)
	* \param data Pointer to the new texture data.
	* \return Returns true if the texture was succesfully created, false otherwise
	*/
	bool reload(GLenum internal_format, int dim_x, int dim_y, GLenum format, GLenum type, GLvoid * data);

private:
	Texture2D(Texture2D &cpy) {}
};

#endif
