#ifndef GLOWL_GLINCLUDE_H
#define GLOWL_GLINCLUDE_H

#if defined(GLOWL_OPENGL_INCLUDE_GLAD)
#include <glad/glad.h>
#elif defined(GLOWL_OPENGL_INCLUDE_GLAD2)
#include <glad/gl.h>
#elif defined(GLOWL_OPENGL_INCLUDE_GL3W)
#include <GL/gl3w.h>
#elif defined(GLOWL_OPENGL_INCLUDE_GLEW)
#include <GL/glew.h>
#endif

#endif // GLOWL_GLINCLUDE_H
