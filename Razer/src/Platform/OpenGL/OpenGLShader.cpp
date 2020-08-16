#include "RZPCH.h"
#include "OpenGLShader.h"

#include "Razer/Log.h"
#include <glad/glad.h>

namespace rz {
	static ShaderDataType OpenGLEnumToShaderDataType(GLenum type) {
		switch(type) {
			case GL_NONE:                  return ShaderDataType::None;
			case GL_BOOL:                  return ShaderDataType::BOOL;
			case GL_UNSIGNED_INT:          return ShaderDataType::UINT;
			case GL_UNSIGNED_INT_VEC2:     return ShaderDataType::UINT2;
			case GL_UNSIGNED_INT_VEC3:     return ShaderDataType::UINT3;
			case GL_UNSIGNED_INT_VEC4:     return ShaderDataType::UINT4;
			case GL_INT:                   return ShaderDataType::INT;
			case GL_INT_VEC2:              return ShaderDataType::INT2;
			case GL_INT_VEC3:              return ShaderDataType::INT3;
			case GL_INT_VEC4:              return ShaderDataType::INT4;
			case GL_FLOAT:                 return ShaderDataType::FLOAT;
			case GL_FLOAT_VEC2:            return ShaderDataType::FLOAT2;
			case GL_FLOAT_VEC3:            return ShaderDataType::FLOAT3;
			case GL_FLOAT_VEC4:            return ShaderDataType::FLOAT4;
			case GL_FLOAT_MAT2:            return ShaderDataType::MAT2;
			case GL_FLOAT_MAT3:            return ShaderDataType::MAT3;
			case GL_FLOAT_MAT4:            return ShaderDataType::MAT4;
		}
		RZ_CORE_ASSERT(false, "This GLenum Type not supported");
		return ShaderDataType::None;
	}


	OpenGLShader::OpenGLShader(const std::string& vertSrc, const std::string& fragSrc) {
		Init(vertSrc, fragSrc);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::Init(const std::string& vertSrc, const std::string& fragSrc) {
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertSrc.c_str();
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
			RZ_CORE_ERROR("{0}", infoLog.data());
			RZ_CORE_ASSERT(false, "Shader:: VetexShader: Compilation Failed");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragSrc.c_str();
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
			RZ_CORE_ERROR("{0}", infoLog.data());
			RZ_CORE_ASSERT(false, "Shader:: FragmentShader: Compilation Failed");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			RZ_CORE_ERROR("{0}", infoLog.data());
			RZ_CORE_ASSERT(false, "Shader:: Program: Link Failed")
				return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);

		// Compute Layout of Shader
		ComputeLayout();
	}

	void OpenGLShader::ComputeLayout() {
		std::vector<BufferElement> elements;

		GLint count = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTES, &count);

		GLint maxSize = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxSize);

		GLenum type;
		GLint size;
		GLsizei length;

		std::vector<GLchar> name(maxSize);

		for (GLint i = 0; i < count; i++) {
			glGetActiveAttrib(m_RendererID, (GLuint)i, maxSize, &length, &size, &type, &name[0]);
			
			BufferElement element(OpenGLEnumToShaderDataType(type), std::string(name.data()));
			elements.push_back(element);
		}

		m_Layout = std::shared_ptr<BufferLayout>(new BufferLayout(elements));

		// DEBUG:: Print Info
		PrintInfo();
	}

	void OpenGLShader::PrintInfo() {
		GLint i, count, size;
		GLenum type;
		GLint bufSize = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize);
		GLchar* name = new GLchar[bufSize];
		GLsizei length;

		glGetProgramiv(m_RendererID, GL_ACTIVE_ATTRIBUTES, &count);
		RZ_CORE_WARN("Active Attribute: {0}", count);
		for (i = 0; i < count; i++) {
			glGetActiveAttrib(m_RendererID, (GLuint)i, bufSize, &length, &size, &type, &name[0]);
			RZ_CORE_WARN("Attrribute: {0} Type: {1} Name: {2} Size: {3} Length: {4} BufSize: {5}", i, type, name, size, length, bufSize);
		}
		delete name;
	}
}

