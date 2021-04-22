/*
 * GLSLProgram.hpp
 *
 * MIT License
 * Copyright (c) 2019 Michael Becher
 */

#ifndef GLOWL_GLSLPROGRAM_HPP
#define GLOWL_GLSLPROGRAM_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#if __has_include(<glm/glm.hpp>)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLOWL_USE_GLM 1
#else
#define GLOWL_USE_GLM 0
#endif

#include "Exceptions.hpp"
#include "glinclude.h"

namespace glowl
{
    /**
     * \class GLSLProgram
     *
     * \brief Encapsulates shader program functionality.
     *
     * \author Michael Becher
     */
    class GLSLProgram
    {
    public:
        enum class ShaderType
        {
            // clang-format off
            Vertex         = GL_VERTEX_SHADER,
            TessControl    = GL_TESS_CONTROL_SHADER,
            TessEvaluation = GL_TESS_EVALUATION_SHADER,
            Geometry       = GL_GEOMETRY_SHADER,
            Fragment       = GL_FRAGMENT_SHADER,
            Compute        = GL_COMPUTE_SHADER
            // clang-format on
        };

        typedef std::vector<std::pair<GLSLProgram::ShaderType, std::string>> ShaderSourceList;

        /**
         * \brief GLSLProgram constructor.
         *
         * Note: Active OpenGL context required for construction.
         * Use std::unqiue_ptr (or shared_ptr) for delayed construction of class member variables of this type.
         */
        GLSLProgram(ShaderSourceList const& shaderList);
        /**
         * \brief GLSLProgram constructor.
         *
         * Wraps (and takes ownership of) an existing shader program.
         */
        GLSLProgram(GLuint handle);
        ~GLSLProgram();

        // Deleted copy constructor (C++11). No going around deleting copies of OpenGL Object with identical handles!
        GLSLProgram(GLSLProgram const& cpy) = delete;
        GLSLProgram(GLSLProgram&& other) = delete;
        GLSLProgram& operator=(GLSLProgram const& rhs) = delete;
        GLSLProgram& operator=(GLSLProgram&& rhs) = delete;

        /**
         * \brief Calls glUseProgram.
         */
        void use();

        /**
         * \brief Returns the OpenGL handle of the program. Handle with care!
         */
        GLuint getHandle();

        /**
         * \brief Associate a vertex shader attribute variable with a specific vertex attribute index.
         * Useful if mesh vertex attribute order is different from order given in vertex shader.
         */
        void bindAttribLocation(GLuint location, GLchar const* name);

        /**
         * \brief Associate several vertex shader attribute variables with specific vertex attribute indices.
         * Useful if mesh vertex attribute order is different from order given in vertex shader.
         * \param location_name_pairs A vector of pairs of location (i.e. vertex attribute index) and vertex shader attribute variable name
         */
        void bindAttribLocations(std::vector<std::pair<GLuint, std::string>> const& location_name_pairs);

        /**
         * \brief Associates a fragment shader output variable with a specific output index.
         * Ignored if output locations statically defined in shader.
         */
        void bindFragDataLocation(GLuint location, char const* name);

        /**
         * \brief Associates fragment shader output variables with specific output indices.
         * Ignored if output locations statically defined in shader.
         * \param location_name_pairs A vector of pairs of location (i.e. output index) and fragment shader output variable name
         */
        void bindFragDataLocations(std::vector<std::pair<GLuint, std::string>> const& location_name_pairs);

        void setUniform(GLchar const* name, GLfloat v0);
        void setUniform(GLchar const* name, GLfloat v0, GLfloat v1);
        void setUniform(GLchar const* name, GLfloat v0, GLfloat v1, GLfloat v2);
        void setUniform(GLchar const* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
        void setUniform(GLchar const* name, GLint v0);
        void setUniform(GLchar const* name, GLint v0, GLint v1);
        void setUniform(GLchar const* name, GLint v0, GLint v1, GLint v2);
        void setUniform(GLchar const* name, GLint v0, GLint v1, GLint v2, GLint v3);
        void setUniform(GLchar const* name, GLuint v0);
        void setUniform(GLchar const* name, GLuint v0, GLuint v1);
        void setUniform(GLchar const* name, GLuint v0, GLuint v1, GLuint v2);
        void setUniform(GLchar const* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
#if GLOWL_USE_GLM
        void setUniform(GLchar const* name, glm::vec2 const& v);
        void setUniform(GLchar const* name, glm::vec3 const& v);
        void setUniform(GLchar const* name, glm::vec4 const& v);
        void setUniform(GLchar const* name, glm::ivec2 const& v);
        void setUniform(GLchar const* name, glm::ivec3 const& v);
        void setUniform(GLchar const* name, glm::ivec4 const& v);
        void setUniform(GLchar const* name, glm::mat2 const& m);
        void setUniform(GLchar const* name, glm::mat3 const& m);
        void setUniform(GLchar const* name, glm::mat4 const& m);
#endif

        /**
         * \brief Return the position of a uniform.
         */
        GLint getUniformLocation(GLchar const* name);

        /**
         * \brief Prints a list if active shader uniforms to std outstream.
         */
        std::string getActiveUniforms();

        /**
         * \brief Prints a list if active shader attributes to std outstream.
         */
        std::string getActiveAttributes();

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
         * \param shaderType Shader type (e.g. GLSLProgam::VertexShader)
         * \param source Shader source code
         */
        void compileShaderFromString(ShaderType shaderType, std::string const& source);

        /**
         * \brief Links program
         */
        void link();

        GLuint      m_handle;      ///< OpenGL program handle
        std::string m_debug_label; ///< An optional label string that is used as glObjectLabel in debug.
    };

    inline GLSLProgram::GLSLProgram(ShaderSourceList const& shaderList)
    {
        m_handle = glCreateProgram();

        try
        {
            for (auto const& shader : shaderList)
            {
                compileShaderFromString(shader.first, shader.second);
            }
            link();
        }
        catch (...)
        {
            glDeleteProgram(m_handle);
            throw;
        }
    }

    inline GLSLProgram::GLSLProgram(GLuint handle) : m_handle(handle) {}

    inline GLSLProgram::~GLSLProgram()
    {
        glDeleteProgram(m_handle);
    }

    inline void GLSLProgram::compileShaderFromString(ShaderType shaderType, std::string const& source)
    {
        // Check if the source is empty.
        if (source.empty())
        {
            throw GLSLProgramException("No shader source.");
        }

        // Create shader object.
        GLchar const* c_source = source.c_str();
        GLuint        shader = glCreateShader(static_cast<GLuint>(shaderType));
        glShaderSource(shader, 1, &c_source, NULL);

        // Compile shader.
        glCompileShader(shader);

        // Check for errors.
        GLint compile_status = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE)
        {
            std::string info_log_str;

            GLint info_log_length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
            if (info_log_length > 0)
            {
                std::vector<GLchar> info_log(info_log_length);
                GLsizei             chars_written;
                glGetShaderInfoLog(shader, info_log_length, &chars_written, info_log.data());
                info_log_str = std::string(info_log.data());
            }

            glDeleteShader(shader);
            throw GLSLProgramException(info_log_str);
        }

        // Attach shader to program.
        glAttachShader(m_handle, shader);

        // Flag shader program for deletion. It will only be actually deleted after the program is deleted.
        glDeleteShader(shader);
    }

    inline void GLSLProgram::link()
    {
        glLinkProgram(m_handle);

        GLint link_status = GL_FALSE;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &link_status);
        if (link_status == GL_FALSE)
        {
            std::string info_log_str;

            GLint info_log_length = 0;
            glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &info_log_length);
            if (info_log_length > 0)
            {
                std::vector<GLchar> info_log(info_log_length);
                GLsizei             chars_written;
                glGetProgramInfoLog(m_handle, info_log_length, &chars_written, info_log.data());
                info_log_str = std::string(info_log.data());
            }
            throw GLSLProgramException(info_log_str);
        }
    }

    inline void GLSLProgram::use()
    {
        glUseProgram(m_handle);
    }

    inline GLuint GLSLProgram::getHandle()
    {
        return m_handle;
    }

    inline void GLSLProgram::bindAttribLocation(GLuint location, GLchar const* name)
    {
        glBindAttribLocation(m_handle, location, name);
        link(); // relink program to apply attrib location binding 
    }

    inline void GLSLProgram::bindAttribLocations(std::vector<std::pair<GLuint, std::string>> const& location_name_pairs)
    {
        for (auto& location_name : location_name_pairs) {
            glBindAttribLocation(m_handle, location_name.first, location_name.second.c_str());
        }
        link(); // relink program to apply attrib location binding 
    }

    inline void GLSLProgram::bindFragDataLocation(GLuint location, char const* name)
    {
        glBindFragDataLocation(m_handle, location, name);
        link(); // relink program to apply frag data location binding 
    }

    inline void GLSLProgram::bindFragDataLocations(std::vector<std::pair<GLuint, std::string>> const& location_name_pairs)
    {
        for (auto& location_name : location_name_pairs)
        {
            glBindFragDataLocation(m_handle, location_name.first, location_name.second.c_str());
        }
        link(); // relink program to apply frag data location binding 
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLfloat v0)
    {
        glUniform1f(getUniformLocation(name), v0);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLfloat v0, GLfloat v1)
    {
        glUniform2f(getUniformLocation(name), v0, v1);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        glUniform3f(getUniformLocation(name), v0, v1, v2);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLint v0)
    {
        glUniform1i(getUniformLocation(name), v0);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLint v0, GLint v1)
    {
        glUniform2i(getUniformLocation(name), v0, v1);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLint v0, GLint v1, GLint v2)
    {
        glUniform3i(getUniformLocation(name), v0, v1, v2);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        glUniform4i(getUniformLocation(name), v0, v1, v2, v3);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLuint v0)
    {
        glUniform1ui(getUniformLocation(name), v0);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLuint v0, GLuint v1)
    {
        glUniform2ui(getUniformLocation(name), v0, v1);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLuint v0, GLuint v1, GLuint v2)
    {
        glUniform3ui(getUniformLocation(name), v0, v1, v2);
    }

    inline void GLSLProgram::setUniform(GLchar const* name, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        glUniform4ui(getUniformLocation(name), v0, v1, v2, v3);
    }

#if GLOWL_USE_GLM
    inline void GLSLProgram::setUniform(GLchar const* name, glm::vec2 const& v)
    {
        glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(GLchar const* name, glm::vec3 const& v)
    {
        glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(GLchar const* name, glm::vec4 const& v)
    {
        glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(GLchar const* name, glm::ivec2 const& v)
    {
        glUniform2iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(GLchar const* name, glm::ivec3 const& v)
    {
        glUniform3iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(GLchar const* name, glm::ivec4 const& v)
    {
        glUniform4iv(getUniformLocation(name), 1, glm::value_ptr(v));
    }

    inline void GLSLProgram::setUniform(GLchar const* name, glm::mat2 const& m)
    {
        glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    inline void GLSLProgram::setUniform(GLchar const* name, glm::mat3 const& m)
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }

    inline void GLSLProgram::setUniform(GLchar const* name, glm::mat4 const& m)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
    }
#endif

    inline GLint GLSLProgram::getUniformLocation(GLchar const* name)
    {
        return glGetUniformLocation(m_handle, name);
    }

    inline std::string GLSLProgram::getActiveUniforms()
    {
        GLint max_length, uniform_cnt;
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &uniform_cnt);
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);

        std::vector<GLchar> attribute_name(max_length);

        GLint   size, location;
        GLsizei written;
        GLenum  type;

        std::stringstream result;

        for (int i = 0; i < uniform_cnt; i++)
        {
            glGetActiveUniform(m_handle, i, max_length, &written, &size, &type, attribute_name.data());
            location = glGetUniformLocation(m_handle, attribute_name.data());
            result << location << " - " << attribute_name.data() << std::endl;
        }
        return result.str();
    }

    inline std::string GLSLProgram::getActiveAttributes()
    {
        GLint max_length, attribute_cnt;
        glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &attribute_cnt);
        glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);

        std::vector<GLchar> attribute_name(max_length);

        GLint  written, size, location;
        GLenum type;

        std::stringstream result;

        for (int i = 0; i < attribute_cnt; i++)
        {
            glGetActiveAttrib(m_handle, i, max_length, &written, &size, &type, attribute_name.data());
            location = glGetAttribLocation(m_handle, attribute_name.data());
            result << location << " - " << attribute_name.data() << std::endl;
        }
        return result.str();
    }

    inline void GLSLProgram::setDebugLabel(std::string const& debug_label)
    {
        m_debug_label = debug_label;
#if _DEBUG
        glObjectLabel(GL_PROGRAM, m_handle, static_cast<GLsizei>(m_debug_label.length()), m_debug_label.c_str());
#endif
    }

    inline std::string GLSLProgram::getDebugLabel() const
    {
        return m_debug_label;
    }

} // namespace glowl

#endif // GLOWL_GLSLPROGRAM_HPP
