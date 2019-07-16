/*
 * Texture3Dview.hpp
 * 
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef Texture3DView_hpp
#define Texture3DView_hpp

#include "Texture.hpp"
#include "Texture3D.hpp"

/**
* \class TextureCubemapArray
*
* \brief Encapsulates 3D texture view functionality.
*
* \author Michael Becher
*/
class Texture3DView : public Texture
{
public:
	Texture3DView(std::string id,
		Texture3D const& source_texture,
		TextureLayout const& layout,
		GLuint minlevel,
		GLuint numlevels,
		GLuint minlayer,
		GLuint numlayers);

	void bindTexture() const;

	void bindImage(GLuint location, GLenum access) const;

	TextureLayout getTextureLayout() const;

	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getDepth();

private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_depth;
};

#endif // !Texture3DView_hpp
