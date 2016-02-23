#ifndef texture3D_h
#define texture3D_h

#include "texture.h"

/**
* \class Texture3D
*
* \brief Encapsulates basic 3D texure functionality.
*
* This class encapsulates basic 3D functionality including creation of a 3D texture,
* as well as setting parameters and binding it.
*
* \author Michael Becher
*
* \date 6th November 2015
*/
class Texture3D : public Texture
{
public:
	Texture3D(std::string name, GLint internal_format, unsigned int width, unsigned int height, unsigned int depth, GLenum format, GLenum type, GLvoid * data);
	Texture3D(const Texture3D &) = delete;

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
	 * \brief Set several texture parameters at once
	 * \param pname_param_pairs List of parameter name and parameter value pairs (e.g. {{GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE},{...,...},...}
	 */
	template<typename Container>
	void texParameteri(const Container& pname_param_pairs)
	{
		glBindTexture(GL_TEXTURE_3D, m_handle);

		for(auto& pname_param : pname_param_pairs)
			glTexParameteri(GL_TEXTURE_3D, pname_param.first, pname_param.second);

		glBindTexture(GL_TEXTURE_3D, 0);
	}

	/**
	* \brief Reload the texture with a new size but unchanged format and type.
	* \note Will not work if load() hasn't been called on this object yet.
	* \param width Specifies the new width of the texture in pixels.
	* \param height Specifies the new height of the texture in pixels.
	* \param depth Specifies the new depth of the texture in pixels.
	* \param data Pointer to the new texture data.
	* \return Returns true if the texture was succesfully created, false otherwise
	*/
	bool reload(unsigned int width, unsigned int height, unsigned int depth, GLvoid * data);

	/**
	* \brief Reload the texture with any new format, type and size.
	* \note Will not work if load() hasn't been called on this object yet.
	* \param internal_format Specifies the new internal format of the texture (e.g. GL_RGBA)
	* \param width Specifies the new width of the texture in pixels.
	* \param height Specifies the new height of the texture in pixels.
	* \param depth Specifies the new depth of the texture in pixels
	* \param format Specifies the new format of the texture (e.g. GL_RGBA32F)
	* \param type Specifies the new type of the texture (e.g. GL_FLOAT)
	* \param data Pointer to the new texture data.
	* \return Returns true if the texture was succesfully created, false otherwise
	*/
	bool reload(GLenum internal_format, unsigned int width, unsigned int height, unsigned int depth, GLenum format, GLenum type, GLvoid * data);

	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getDepth();

private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_depth;
};

#endif