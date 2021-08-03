#include "repch.h"
#include "OpenGLShader.h"

#include "repch.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Real {

	//pixelSrc is in OpenGL terms fragmentSrc. Yep, fragment shader.
	//Code from official OpenGL wiki: https://www.khronos.org/opengl/wiki/Shader_Compilation#Example
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& pixelSrc) {

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			RE_CORE_ERROR("{0}", infoLog.data());
			RE_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = pixelSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			RE_CORE_ERROR("{0}", infoLog.data());
			RE_CORE_ASSERT(false, "Pixel shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		rendererId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(rendererId, vertexShader);
		glAttachShader(rendererId, fragmentShader);

		// Link our program
		glLinkProgram(rendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(rendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(rendererId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(rendererId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(rendererId);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			RE_CORE_ERROR("{0}", infoLog.data());
			RE_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(rendererId, vertexShader);
		glDetachShader(rendererId, fragmentShader);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererId);
	}
	void OpenGLShader::bind() const {
		glUseProgram(rendererId);
	}
	void OpenGLShader::unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int value) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform1i(location, value);
	}

	
	void OpenGLShader::uploadUniformFloat(const std::string& name, float value) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform1f(location, value);
	}
	
	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& vector) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform2f(location, vector.x, vector.y);
	}
	
	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& vector) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& vector) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	

	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const {
		//glUseProgram(rendererId);
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
