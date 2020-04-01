/*
 * GLSLProgram.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLSLProgram_h
#define GLSLProgram_h

#include <iostream>
#include <vector>

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
        enum ShaderType { 
            VertexShader           = GL_VERTEX_SHADER, 
            TessellationControl    = GL_TESS_CONTROL_SHADER,
            TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
            GeometryShader         = GL_GEOMETRY_SHADER, 
            FragmentShader         = GL_FRAGMENT_SHADER, 
            ComputeShader          = GL_COMPUTE_SHADER};

        /**
        * \brief GLSLProgram constructor.
        *
        * Note: Active OpenGL context required for construction.
        * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
        */
        GLSLProgram(std::vector<std::pair<GLSLProgram::ShaderType, std::string>> const& shaders, bool verbose = false);
        ~GLSLProgram();

        /* Deleted copy constructor (C++11). No going around deleting copies of OpenGL Object with identical handles! */
        GLSLProgram(const GLSLProgram& cpy) = delete;
        GLSLProgram(GLSLProgram&& other) = delete;
        GLSLProgram& operator=(const GLSLProgram& rhs) = delete;
        GLSLProgram& operator=(GLSLProgram&& rhs) = delete;

        /**
         * \brief Calls glUseProgram
         */
        bool use();

        /**
         * \brief Gives access to log file that contain details about program and shader creation
         */
        std::string const& getLog();

        /**
         * \brief Returns the OpenGL handle of the program. Handle with care!
         */
        GLuint getHandle();

        /**
         * \brief Allows to check whether a compute shader is attached to program for sanity check before calling glDispatchCompute
         */
        bool hasComputeShader();

        /**
         * \brief Associate a vertex shader attribute variable with a specific vertex attribute index.
         * Useful if mesh vertex attribute order is different from order given in vertex shader.
         */
        void bindAttribLocation(GLuint location, const GLchar* name);

        /**
         * \brief Associates a fragment shader output variable with a specific output index.
         * Ignored if output locations statically defined in shader.
         */
        void bindFragDataLocation(GLuint location, const char* name);

        void setUniform(const GLchar* name, GLfloat v0);
        void setUniform(const GLchar* name, GLfloat v0, GLfloat v1);
        void setUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2);
        void setUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
        void setUniform(const GLchar* name, GLint v0);
        void setUniform(const GLchar* name, GLint v0, GLint v1);
        void setUniform(const GLchar* name, GLint v0, GLint v1, GLint v2);
        void setUniform(const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3);
        void setUniform(const GLchar* name, GLuint v0);
        void setUniform(const GLchar* name, GLuint v0, GLuint v1);
        void setUniform(const GLchar* name, GLuint v0, GLuint v1, GLuint v2);
        void setUniform(const GLchar* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
        void setUniform(const GLchar* name, const glm::vec2& v);
        void setUniform(const GLchar* name, const glm::vec3& v);
        void setUniform(const GLchar* name, const glm::vec4& v);
        void setUniform(const GLchar* name, const glm::ivec2& v);
        void setUniform(const GLchar* name, const glm::ivec3& v);
        void setUniform(const GLchar* name, const glm::ivec4& v);
        void setUniform(const GLchar* name, const glm::mat2& m);
        void setUniform(const GLchar* name, const glm::mat3& m);
        void setUniform(const GLchar* name, const glm::mat4& m);

        /**
         * \brief Return the position of a uniform.
         */
        GLuint getUniformLocation(const GLchar* name);

        /**
         * \brief Prints a list if active shader uniforms to std outstream.
         */
        void printActiveUniforms();

        /**
         * \brief Prints a list if active shader attributes to std outstream.
         */
        void printActiveAttributes();

        /**
         * \brief Set a debug label to be used as glObjectLabel in debug
         */
        void setDebugLabel(std::string const& debug_label);

        /**
         * \brief Returns debug label string
         */
        std::string getDebugLabel() const;

    private:
        /**
         * \brief Compiles and attaches a shader program
         * \param source Shader source code
         * \param shaderType Shader type (e.g. GLSLProgam::VertexShader)
         * \return Returns true if shader was succefully compiled, false otherwise.
         */
        bool compileShaderFromString(std::string const& source, ShaderType shaderType);

        /**
         * \brief Links program
         * \return Returns true if shader was succefully linked, false otherwise.
         */
        bool link();

        /** OpenGL program handle */
        GLuint      m_handle;
        /** Keeps track if a compute shader was attached to progam */
        bool        m_compute_shader;
        /** Log with all outputs from program and shader generation */
        std::string m_shaderlog;
        /** An optional label string that is used as glObjectLabel in debug */
        std::string m_debug_label;
    };

    inline GLSLProgram::GLSLProgram(std::vector<std::pair<GLSLProgram::ShaderType, std::string>> const& shaders, bool verbose) : m_compute_shader(false), m_shaderlog("")
    {
        m_handle = glCreateProgram();

        for (auto const& shader : shaders) {
            if (!compileShaderFromString(shader.second, shader.first)) {
                if (verbose) {
                    std::cerr << m_shaderlog << std::endl;
                }
                throw std::runtime_error("Error compiling shader!");
            }
        }

        if (!link()) {
            if (verbose) {
                std::cerr << m_shaderlog << std::endl;
            }
            throw std::runtime_error("Error linking shader program!");
        }
    }

    inline GLSLProgram::~GLSLProgram()
    {
        glDeleteProgram(m_handle);
    }

    inline bool GLSLProgram::compileShaderFromString(std::string const& source, ShaderType shaderType)
    {
        /* Check if the source is empty */
        if (source.empty()) {
            m_shaderlog = "No shader source.";
            return false;
        }

        /* Create shader object */
        const GLchar* c_source = source.c_str();
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &c_source, NULL);

        if (shaderType == GL_COMPUTE_SHADER) m_compute_shader = true;

        /* Compile shader */
        glCompileShader(shader);

        /* Check for errors */
        GLint compileStatus = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE) {
            GLint infoLogLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0) {
                std::vector<GLchar> infoLog(infoLogLength);
                GLsizei charsWritten;
                glGetShaderInfoLog(shader, infoLogLength, &charsWritten, infoLog.data());
                std::string infoLogStr(infoLog.data());
                m_shaderlog.append(infoLogStr);
            }

            glDeleteShader(shader);
            return false;
        }

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
        glLinkProgram(m_handle);

        GLint linkStatus = GL_FALSE;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE) {
            GLint infoLogLength = 0;
            glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0) {
                std::vector<GLchar> infoLog(infoLogLength);
                GLsizei charsWritten;
                glGetProgramInfoLog(m_handle, infoLogLength, &charsWritten, infoLog.data());
                std::string infoLogStr(infoLog.data());
                m_shaderlog.append(infoLogStr);
            }
            return false;
        }

        return true;
    }

    inline bool GLSLProgram::use()
    {
        glUseProgram(m_handle);
        return true;
    }

    inline std::string const& GLSLProgram::getLog()
    {
        return m_shaderlog;
    }

    inline GLuint GLSLProgram::getHandle()
    {
        return m_handle;
    }

    inline bool GLSLProgram::hasComputeShader()
    {
        return m_compute_shader;
    }

    inline void GLSLProgram::bindAttribLocation(GLuint location, const GLchar* name)
    {
        glBindAttribLocation(m_handle, location, name);
    }

    inline void GLSLProgram::bindFragDataLocation(GLuint location, const char* name)
    {
        glBindFragDataLocation(m_handle, location, name);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLfloat v0)
    {
        glUniform1f(getUniformLocation(name), v0);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLfloat v0, GLfloat v1)
    {
        glUniform2f(getUniformLocation(name), v0, v1);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        glUniform3f(getUniformLocation(name), v0, v1, v2);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLint v0)
    {
        glUniform1i(getUniformLocation(name), v0);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLint v0, GLint v1)
    {
        glUniform2i(getUniformLocation(name), v0, v1);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLint v0, GLint v1, GLint v2)
    {
        glUniform3i(getUniformLocation(name), v0, v1, v2);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        glUniform4i(getUniformLocation(name), v0, v1, v2, v3);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLuint v0)
    {
        glUniform1ui(getUniformLocation(name), v0);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLuint v0, GLuint v1)
    {
        glUniform2ui(getUniformLocation(name), v0, v1);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLuint v0, GLuint v1, GLuint v2)
    {
        glUniform3ui(getUniformLocation(name), v0, v1, v2);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        glUniform4ui(getUniformLocation(name), v0, v1, v2, v3);
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::vec2& v)
    {
        glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::vec3& v)
    {
        glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::vec4& v)
    {
        glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::ivec2& v)
    {
        glUniform2iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::ivec3& v)
    {
        glUniform3iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::ivec4& v)
    {
        glUniform4iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::mat2& m)
    {
        glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::mat3& m)
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    inline void GLSLProgram::setUniform(const GLchar* name, const glm::mat4& m)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    inline GLuint GLSLProgram::getUniformLocation(const GLchar* name)
    {
        return glGetUniformLocation(m_handle, name);
    }

    inline void GLSLProgram::printActiveUniforms()
    {
        GLint maxLength, nUniforms;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &nUniforms);
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

        GLchar* attributeName = (GLchar*) new char[maxLength];

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

        GLchar* attributeName = (GLchar*) new char[maxLength];

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

    inline void GLSLProgram::setDebugLabel(const std::string & debug_label)
    {
        m_debug_label = debug_label;
#if _DEBUG
        glObjectLabel(GL_PROGRAM, this->m_handle, static_cast<GLsizei>(m_debug_label.length()), m_debug_label.c_str());
#endif
    }

    inline std::string GLSLProgram::getDebugLabel() const
    {
        return m_debug_label;
    }

}

#endif // !GLSLProgram_hpp
