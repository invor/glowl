/*
 * GLSLProgram.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLSLProgram_h
#define GLSLProgram_h

#include <iostream>

#include <glad/glad.h>
 //	OpenGL Math Library
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


namespace glowl
{

    /**
    * \class GLSLProgram
    *
    * \brief Encapsulates shader program functionality. Possibly somewhat outdated.
    *
    * \author Michael Becher
    */
    class GLSLProgram
    {
    public:
        enum ShaderType { VertexShader, TessellationControl, TessellationEvaluation, GeometryShader, FragmentShader, ComputeShader };

    private:
        GLuint m_handle;
        bool m_link_status;
        bool m_compute_shader;
        std::string m_shaderlog;

        // TODO: this is simply a hotfix solution
        std::string m_id;

        GLuint getUniformLocation(const char *name);
    public:
        /**
        * \brief GLSLProgram constructor.
        *
        * Note: Active OpenGL context required for construction.
        * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
        */
        GLSLProgram();
        ~GLSLProgram();

        /* Deleted copy constructor (C++11). No going around deleting copies of OpenGL Object with identical handles! */
        GLSLProgram(const GLSLProgram& cpy) = delete;

        GLSLProgram(GLSLProgram&& other) = delete;

        GLSLProgram& operator=(const GLSLProgram& rhs) = delete;
        GLSLProgram& operator=(GLSLProgram&& rhs) = delete;

        void init();
        bool compileShaderFromString(const std::string * const source, GLenum shaderType);
        bool link();
        bool use();
        bool dispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
        const std::string& getLog();
        GLuint getHandle();
        bool isLinked();
        bool isComputeShader();
        void bindAttribLocation(GLuint location, const char *name);
        void bindFragDataLocation(GLuint location, const char *name);

        void setUniform(const char *name, const glm::vec2 &v);
        void setUniform(const char *name, const glm::ivec2 &v);
        void setUniform(const char *name, const glm::ivec3 &v);
        void setUniform(const char *name, const glm::ivec4 &v);
        void setUniform(const char *name, const glm::vec3 &v);
        void setUniform(const char *name, const glm::vec4 &v);
        void setUniform(const char *name, const glm::mat4 &m);
        void setUniform(const char *name, const glm::mat3 &m);
        void setUniform(const char *name, int i);
        void setUniform(const char *name, unsigned int i);
        void setUniform(const char *name, float f);
        void setUniform(const char *name, bool b);
        void printActiveUniforms();
        void printActiveAttributes();

        void setId(const std::string& id);
        std::string getId() const;
    };

    inline GLSLProgram::GLSLProgram() : m_link_status(false), m_compute_shader(false)
    {
    }

    inline GLSLProgram::~GLSLProgram()
    {
        glDeleteProgram(m_handle);
    }

    inline GLuint GLSLProgram::getUniformLocation(const char *name)
    {
        return glGetUniformLocation(m_handle, name);
    }

    inline void GLSLProgram::init()
    {
        m_handle = glCreateProgram();
    }

    inline bool GLSLProgram::compileShaderFromString(const std::string * const source, GLenum shaderType)
    {
        /* Check if the source is empty */
        if (source->empty())
        {
            m_shaderlog = "No shader source.";
            return false;
        }

        /* Create shader object */
        const GLchar* c_source = source->c_str();
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &c_source, NULL);

        if (shaderType == GL_COMPUTE_SHADER) m_compute_shader = true;

        /* Compile shader */
        glCompileShader(shader);

        /* Check for errors */
        GLint compile_ok = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
        if (compile_ok == GL_FALSE)
        {
            GLint logLen = 0;
            m_shaderlog = "";
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0)
            {
                char *log = new char[logLen];
                GLsizei written;
                glGetShaderInfoLog(shader, logLen, &written, log);
                m_shaderlog = log;
                delete[] log;
            }

            glDeleteShader(shader);
            return false;
        }

        //	GLint logLen = 0;
        //	m_shaderlog = "";
        //	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        //	if(logLen > 0)
        //	{
        //		char *log = new char[logLen];
        //		GLsizei written;
        //		glGetShaderInfoLog(shader, logLen, &written, log);
        //		m_shaderlog = log;
        //		delete [] log;
        //	}

        /* Attach shader to program */
        glAttachShader(m_handle, shader);
        /* Flag shader program for deletion.
         * It will only be actually deleted after the program is deleted. (See destructor for program deletion.
         */
        glDeleteShader(shader);

        return true;
    }

    inline bool GLSLProgram::link()
    {
        if (m_link_status) return true;
        glLinkProgram(m_handle);

        GLint status = GL_FALSE;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint logLen = 0;
            //m_shaderlog = "";
            glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0)
            {
                char *log = new char[logLen];
                GLsizei written;
                glGetProgramInfoLog(m_handle, logLen, &written, log);
                m_shaderlog.append(log);
                delete[] log;
            }
            return false;
        }

        //	GLint logLen = 0;
        //	m_shaderlog = "";
        //	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLen);
        //	if(logLen > 0)
        //	{
        //		char *log = new char[logLen];
        //		GLsizei written;
        //		glGetProgramInfoLog(m_handle, logLen, &written, log);
        //		m_shaderlog = log;
        //		delete [] log;
        //	}
        //	return false;

        m_link_status = true;
        return m_link_status;
    }

    inline bool GLSLProgram::use()
    {
        if (!m_link_status) return false;

        glUseProgram(m_handle);

        return true;
    }

    inline bool GLSLProgram::dispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
    {
        GLuint current_prgm;
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&current_prgm);

        if ((current_prgm != m_handle) || !m_compute_shader)
            return false;

        glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);

        return true;
    }

    inline const std::string& GLSLProgram::getLog()
    {
        return m_shaderlog;
    }

    inline GLuint GLSLProgram::getHandle()
    {
        return m_handle;
    }

    inline bool GLSLProgram::isLinked()
    {
        return m_link_status;
    }

    inline void GLSLProgram::bindAttribLocation(GLuint location, const char *name)
    {
        glBindAttribLocation(m_handle, location, name);
    }

    inline void GLSLProgram::bindFragDataLocation(GLuint location, const char *name)
    {
        glBindFragDataLocation(m_handle, location, name);
    }

    inline void GLSLProgram::setUniform(const char *name, const glm::vec2 &v)
    {
        glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const char *name, const glm::ivec2 &v)
    {
        glUniform2iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const char *name, const glm::ivec3 &v)
    {
        glUniform3iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const char *name, const glm::ivec4 &v)
    {
        glUniform4iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const char *name, const glm::vec3 &v)
    {
        glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const char *name, const glm::vec4 &v)
    {
        glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const char *name, const glm::mat4 &m)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    inline void GLSLProgram::setUniform(const char *name, const glm::mat3 &m)
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    inline void GLSLProgram::setUniform(const char *name, int i)
    {
        glUniform1i(getUniformLocation(name), i);
    }

    inline void GLSLProgram::setUniform(const char *name, unsigned int i)
    {
        glUniform1ui(getUniformLocation(name), i);
    }

    inline void GLSLProgram::setUniform(const char *name, float f)
    {
        glUniform1f(getUniformLocation(name), f);
    }

    inline void GLSLProgram::setUniform(const char *name, bool b)
    {
        glUniform1i(getUniformLocation(name), b);
    }

    inline void GLSLProgram::printActiveUniforms()
    {
        GLint maxLength, nUniforms;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &nUniforms);
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

        GLchar * attributeName = (GLchar *) new char[maxLength];

        GLint size, location;
        GLsizei written;
        GLenum type;

        for (int i = 0; i < nUniforms; i++)
        {
            glGetActiveUniform(m_handle, i, maxLength, &written, &size, &type, attributeName);
            location = glGetUniformLocation(m_handle, attributeName);
            std::cout << location << " - " << attributeName << "\n";
        }
        delete[] attributeName;
    }

    inline void GLSLProgram::printActiveAttributes()
    {
        GLint maxLength, nAttributes;
        glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &nAttributes);
        glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

        GLchar * attributeName = (GLchar *) new char[maxLength];

        GLint written, size, location;
        GLenum type;

        for (int i = 0; i < nAttributes; i++)
        {
            glGetActiveAttrib(m_handle, i, maxLength, &written, &size, &type, attributeName);
            location = glGetAttribLocation(m_handle, attributeName);
            std::cout << location << " - " << attributeName << "\n";
        }
        delete[] attributeName;
    }

    inline void GLSLProgram::setId(const std::string& id)
    {
        m_id = id;
    }

    inline std::string GLSLProgram::getId() const
    {
        return m_id;
    }

}

#endif // !GLSLProgram_hpp
