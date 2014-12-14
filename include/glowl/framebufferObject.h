#ifndef framebufferObject_h
#define framebufferObject_h

/*	Include space-lion files */
#include "texture2D.h"

/*	Include system libraries */
#include <vector>
#include <iostream>
#include <string>
#include <memory>


/**
* \class FramebufferObject
*
* \brief Encapsulates basic framebuffer object functionality.
*
* This class encapsulates basic framebuffer object functionality including creation of an FBO,
* creation and adding of several color attachments and use it for rendering.
*
* \author Michael Becher
*
* \date 11th November 2014
*/
class FramebufferObject
{
private:
	/*	Private copy constructor. Don't wanna go around copying objects with OpenGL handles. */
	FramebufferObject(FramebufferObject&);

	/** Handle of the FBO */
	GLuint m_handle;
	/** Colorbuffers attached to the FBO */
	std::vector<std::shared_ptr<Texture2D>> m_colorbuffers;
	/** Handle of the depthbuffer */
	GLuint m_depthbuffer;
	/** Handle of the stencilbuffer */
	GLuint m_stencilbuffer;

	/** Width of the framebuffer i.e. it's color attachments */
	int m_width;
	/** Height of the framebuffer i.e. it's color attachments */
	int m_height;

public:
	FramebufferObject();
	~FramebufferObject();

	FramebufferObject(int width, int height, bool has_depth = false, bool has_stencil = false);

	/**
	* \brief Adds one color attachment to the framebuffer.
	* \note New colorbuffers are added at the end of the colorbuffer vector.
			The index i within the storage vector and the used attachment point
			GL_COLOR_ATTACHMENTi will be the same.
	* \param internalFormat Specifies channels+size (e.g. GL_R32F)
	* \param format Specifies channels (e.g. GL_RED)
	* \param type Specifies datatype (e.g. GL_FLOAT)
	* \return Returns true if a color attachment was added, false otherwise
	*/
	bool createColorAttachment(GLenum internalFormat, GLenum format, GLenum type);

	/**
	* \brief Bind this framebuffer object.
	* \note Checks framebuffer completeness before binding and sets glDrawBuffers
			including all available colobuffers in it.
	*/
	void bind();

	/**
	 * \brief Bind the framebuffer to GL_READ_FRAMEBUFFER
	 * \param index Set glReadBuffer to color attachment #index or 0, if index > #color attachments
	 */
	void bindToRead(unsigned int index);

	/**
	 * \brief Bind the framebuffer to GL_DRAW_FRAMEBUFFER using all color attachments for glDrawBuffers
	 */
	void bindToDraw();

	/**
	* \brief Bind a color attachment as GL_TEXTURE_2D.
	* \param index Specifies which color is bound. If index > #attachments, the method simply won't bind a texture.
	*/
	void bindColorbuffer(unsigned int index);

	/**
	* \brief Bind the depth buffer as GL_TEXTURE_2D.
	* \note Work in progress. Use of this method is discouraged.
	*/
	void bindDepthbuffer();

	/**
	* \brief Bind the stencil buffer as GL_TEXTURE_2D.
	* \note Work in progress. Use of this method is higly discouraged.
	*/
	void bindStencilbuffer();

	/**
	* \brief Check the framebuffer object for completeness.
	* \return Returns true if the framebuffer object is complete, false otherwise.
	*/
	bool checkStatus();

	/**
	* \brief Resize the framebuffer object, i.e. it's color attachments.
	* \note Might be a bit costly to use often.
	* \param new_width Specifies the new framebuffer width.
	* \param new_width Specifies the new framebuffer height.
	*/
	void resize(int new_width, int new_height);

	/**
	* \brief Get the width of the framebuffer object's color attachments
	* \return Returns widths.
	*/
	int getWidth() {return m_width;}

	/**
	* \brief Get the height of the framebuffer object's color attachments
	* \return Returns height.
	*/
	int getHeight() {return m_height;}
};

#endif
